//
// Created by paul.trampert on 11/8/2020.
//

#include <stdexcept>
#include <cmath>
#include <GenieSys/BriefExtensionWord.h>
#include <GenieSys/ExtensionWord.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/signExtend.h>

M68kCpu::M68kCpu() = default;

M68kCpu::~M68kCpu() = default;

void M68kCpu::ConnectBus(Bus *bus) {
    this->bus = bus;
}

void M68kCpu::dataRegisterDirect() {
    address = opWord & EA_REG_MASK;
}

void M68kCpu::addressRegisterDirect() {
    address = opWord & EA_REG_MASK;
}

void M68kCpu::addressRegisterIndirect() {
    address = addressRegisters[opWord & EA_REG_MASK];
}

void M68kCpu::addressRegisterIndirectPostIncrement() {
    uint8_t regAddr = opWord & EA_REG_MASK;
    address = addressRegisters[regAddr];
    uint8_t size = 1;
    switch (operandSize) {
        case BYTE:
            size = regAddr == USP_ADDRESS ? 2 : 1;
            break;
        case WORD:
            size = 2;
            break;
        case LONG:
            size = 4;
            break;
    }
    addressRegisters[regAddr] += size;
}

void M68kCpu::addressRegisterIndirectPreDecrement() {
    uint8_t regAddr = opWord & EA_REG_MASK;
    uint8_t size = 1;
    switch (operandSize) {
        case BYTE:
            size = regAddr == USP_ADDRESS ? 2 : 1;
            break;
        case WORD:
            size = 2;
            break;
        case LONG:
            size = 4;
            break;
    }
    addressRegisters[regAddr] -= size;
    address = addressRegisters[regAddr];
}

void M68kCpu::addressRegisterIndirectDisplacement() {
    auto displacement = signExtend<int32_t>(bus->readWord(pc), 16);
    pc += 2;
    address = addressRegisters[opWord & EA_REG_MASK] + displacement;
}

void M68kCpu::addressRegisterIndirectWithIndex() {
    uint16_t extWordRaw = bus->readWord(pc);
    pc += 2;
    if ((extWordRaw & 0x0100) > 0) {
        auto extWord = decodeBriefExtensionWord(extWordRaw);
        uint32_t addrReg = addressRegisters[opWord & EA_REG_MASK];
        int32_t displacement = signExtend<int32_t>(extWord.displacement, 8);
        int32_t idxReg = extWord.idxRegType == M68K_REG_TYPE_DATA
                ? dataRegisters[extWord.idxRegAddr]
                : addressRegisters[extWord.idxRegAddr];
        if (extWord.idxSize == EXT_WORD_IDX_SIZE_SE_WORD) {
            idxReg = signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
        }
        uint32_t scale = pow(2, extWord.scale);
        address = addrReg + displacement + (idxReg * scale);
    }
    else {
        auto extWord = decodeExtensionWord(extWordRaw);
        uint32_t addrReg = extWord.baseRegSuppress ? 0 : addressRegisters[opWord & EA_REG_MASK];
        int32_t idxReg = 0;
        if (!extWord.indexSuppress) {
            idxReg = extWord.idxRegType == M68K_REG_TYPE_DATA
                    ? dataRegisters[extWord.idxRegAddr]
                    : addressRegisters[extWord.idxRegAddr];
            if (extWord.idxSize == EXT_WORD_IDX_SIZE_SE_WORD) {
                idxReg = signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
            }
        }
        uint32_t scale = pow(2, extWord.scale);
        int32_t baseDisplacement = 0;
        if (extWord.baseDisplacementSize == EXT_WORD_BD_SIZE_WORD) {
            baseDisplacement = signExtend<int32_t>(bus->readWord(pc), 16);
            pc += 2;
        }
        else if (extWord.baseDisplacementSize == EXT_WORD_BD_SIZE_LONG) {
            baseDisplacement = bus->readLong(pc);
            pc += 4;
        }
        int32_t outerDisplacement = 0;
        uint32_t intermediateAddr = 0;
        switch(extWord.indexIndirectSelection) {
            case 0:
                // Address Register Indirect Mode
                address = addrReg + baseDisplacement + (idxReg * scale);
                break;
            case 1:
            case 2:
            case 3:
                // Preindexed
                intermediateAddr = addrReg + baseDisplacement + (idxReg * scale);
                if (extWord.indexIndirectSelection == 2) {
                    outerDisplacement = signExtend<int32_t>(bus->readWord(pc), 16);
                    pc += 2;
                }
                else if (extWord.indexIndirectSelection == 3) {
                    outerDisplacement = bus->readLong(pc);
                    pc += 4;
                }
                address = bus->readLong(intermediateAddr) + outerDisplacement;
                break;
            case 5:
            case 6:
            case 7:
                // Postindexed
                intermediateAddr = addrReg + baseDisplacement;
                if (extWord.indexIndirectSelection == 6) {
                    outerDisplacement = signExtend<int32_t>(bus->readWord(pc), 16);
                    pc += 2;
                }
                else if (extWord.indexIndirectSelection == 7) {
                    outerDisplacement = bus->readLong(pc);
                    pc += 4;
                }
                address = bus->readLong(intermediateAddr) + (idxReg * scale) + outerDisplacement;
                break;
            default:
                throw std::runtime_error("Invalid instruction");
        }
    }
}

void M68kCpu::programCounterIndirectDisplacement() {
    address = pc + signExtend<int32_t>((int32_t)bus->readWord(pc), 16);
    pc += 2;
}

void M68kCpu::programCounterIndirectWithIndex() {
    uint32_t baseAddr = pc;
    uint16_t extWordRaw = bus->readWord(pc);
    pc += 2;
    if ((extWordRaw & 0x0100) > 0) {
        auto extWord = decodeBriefExtensionWord(extWordRaw);
        auto displacement = signExtend<int32_t>(extWord.displacement, 8);
        int32_t idxReg = extWord.idxRegType == M68K_REG_TYPE_DATA
                         ? dataRegisters[extWord.idxRegAddr]
                         : addressRegisters[extWord.idxRegAddr];
        if (extWord.idxSize == EXT_WORD_IDX_SIZE_SE_WORD) {
            idxReg = signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
        }
        uint32_t scale = pow(2, extWord.scale);
        address = baseAddr + displacement + (idxReg * scale);
    }
    else {
        auto extWord = decodeExtensionWord(extWordRaw);
        baseAddr = extWord.baseRegSuppress ? 0 : baseAddr;
        int32_t idxReg = 0;
        if (!extWord.indexSuppress) {
            idxReg = extWord.idxRegType == M68K_REG_TYPE_DATA
                     ? dataRegisters[extWord.idxRegAddr]
                     : addressRegisters[extWord.idxRegAddr];
            if (extWord.idxSize == EXT_WORD_IDX_SIZE_SE_WORD) {
                idxReg = signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
            }
        }
        uint32_t scale = pow(2, extWord.scale);
        int32_t baseDisplacement = 0;
        if (extWord.baseDisplacementSize == EXT_WORD_BD_SIZE_WORD) {
            baseDisplacement = signExtend<int32_t>(bus->readWord(pc), 16);
            pc += 2;
        }
        else if (extWord.baseDisplacementSize == EXT_WORD_BD_SIZE_LONG) {
            baseDisplacement = bus->readLong(pc);
            pc += 4;
        }
        int32_t outerDisplacement = 0;
        uint32_t intermediateAddr = 0;
        switch(extWord.indexIndirectSelection) {
            case 0:
                // PC Indirect Mode
                address = baseAddr + baseDisplacement + (idxReg * scale);
                break;
            case 1:
            case 2:
            case 3:
                // Preindexed
                intermediateAddr = baseAddr + baseDisplacement + (idxReg * scale);
                if (extWord.indexIndirectSelection == 2) {
                    outerDisplacement = signExtend<int32_t>(bus->readWord(pc), 16);
                    pc += 2;
                }
                else if (extWord.indexIndirectSelection == 3) {
                    outerDisplacement = bus->readLong(pc);
                    pc += 4;
                }
                address = bus->readLong(intermediateAddr) + outerDisplacement;
                break;
            case 5:
            case 6:
            case 7:
                // Postindexed
                intermediateAddr = baseAddr + baseDisplacement;
                if (extWord.indexIndirectSelection == 6) {
                    outerDisplacement = signExtend<int32_t>(bus->readWord(pc), 16);
                    pc += 2;
                }
                else if (extWord.indexIndirectSelection == 7) {
                    outerDisplacement = bus->readLong(pc);
                    pc += 4;
                }
                address = bus->readLong(intermediateAddr) + (idxReg * scale) + outerDisplacement;
                break;
            default:
                throw std::runtime_error("Invalid instruction");
        }
    }
}

void M68kCpu::absoluteShortAddressingMode() {
    address = signExtend<int32_t>(bus->readWord(pc), 16);
    pc += 2;
}

void M68kCpu::absoluteLongAddressingMode() {
    address = bus->readLong(pc);
    pc += 4;
}

void M68kCpu::immediateData() {
    address = pc;
}
