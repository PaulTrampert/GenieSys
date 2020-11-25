//
// Created by paul.trampert on 11/8/2020.
//

#include <stdexcept>
#include "GenieSys/m68000cpu.h"
#include "GenieSys/Bus.h"
#include "GenieSys/signExtend.h"

m68000cpu::BriefExtensionWord m68000cpu::decodeBriefExtensionWord(uint16_t word) {
    return m68000cpu::BriefExtensionWord {
            static_cast<M68K_REG_TYPE>(word >> 15),
            static_cast<uint8_t>((word & 0xE000) >> 12),
            static_cast<EXT_WORD_IDX_SIZE>((word & 0x0800) >> 11),
            static_cast<uint8_t>((word & 0x0600) >> 9),
            static_cast<int8_t>((word & 0x00FF))
    };
}

m68000cpu::ExtensionWord m68000cpu::decodeExtensionWord(uint16_t word) {
    return m68000cpu::ExtensionWord {
            static_cast<M68K_REG_TYPE>(word >> 15),
            static_cast<uint8_t>((word & 0xE000) >> 12),
            static_cast<EXT_WORD_IDX_SIZE>((word & 0x0800) >> 11),
            static_cast<uint8_t>((word & 0x0600) >> 8),
            static_cast<bool>(word & 0x0080),
            static_cast<bool>(word & 0x0040),
            static_cast<EXT_WORD_BD_SIZE>((word & 0x0030) >> 4),
            static_cast<uint8_t>(word & 0x0007)
    };
}

m68000cpu::m68000cpu() = default;

m68000cpu::~m68000cpu() = default;

void m68000cpu::ConnectBus(Bus *bus) {
    this->bus = bus;
}

void m68000cpu::dataRegisterDirect() {
    address = opWord & EA_REG_MASK;
}

void m68000cpu::addressRegisterDirect() {
    address = opWord & EA_REG_MASK;
}

void m68000cpu::addressRegisterIndirect() {
    address = addressRegisters[opWord & EA_REG_MASK];
}

void m68000cpu::addressRegisterIndirectPostIncrement() {
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

void m68000cpu::addressRegisterIndirectPreDecrement() {
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

void m68000cpu::addressRegisterIndirectDisplacement() {
    auto displacement = signExtend<int32_t>(bus->readWord(pc), 16);
    pc += 2;
    address = addressRegisters[opWord & EA_REG_MASK] + displacement;
}

void m68000cpu::addressRegisterIndirectWithIndex() {
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

void m68000cpu::programCounterIndirectDisplacement() {
    address = pc + signExtend<int32_t>((int32_t)bus->readWord(pc), 16);
    pc += 2;
}

void m68000cpu::programCounterIndirectWithIndex() {
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

void m68000cpu::absoluteShortAddressingMode() {
    address = signExtend<int32_t>(bus->readWord(pc), 16);
    pc += 2;
}

void m68000cpu::absoluteLongAddressingMode() {
    address = bus->readLong(pc);
    pc += 4;
}

void m68000cpu::immediateData() {
    address = pc;
}
