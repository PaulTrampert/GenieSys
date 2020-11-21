//
// Created by paul.trampert on 11/8/2020.
//

#include "m68000cpu.h"
#include "Bus.h"

template<class T>
static T signExtend(T x, const int bits) {
    T m = 1;
    m <<= bits - 1;
    return (x ^ m) - m;
}

uint8_t m68000cpu::read(uint32_t addr) {
    return bus->read(addr);
}

void m68000cpu::write(uint32_t addr, uint8_t data) {
    bus->write(addr, data);
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
    uint8_t b1 = read(pc++);
    uint8_t b2 = read(pc++);
    uint16_t displacementRaw = (b1 << 8) | b2;
    int32_t displacement = signExtend(displacementRaw, 32);
    address = addressRegisters[opWord & EA_REG_MASK] + displacement;
}

void m68000cpu::addressRegisterIndirect8BitDisplacement() {
    uint32_t baseAddress = addressRegisters[opWord & EA_REG_MASK];
    auto extWord = decodeBriefExtensionWord((read(pc++) << 8) | read(pc++));
    auto idxRegisters = extWord.idxRegType == M68K_REG_TYPE_DATA ? dataRegisters : addressRegisters;
    int32_t idxRegData = idxRegisters[extWord.idxRegAddr];
    uint8_t scale = pow(2, extWord.scale);
    address = baseAddress + extWord.displacement + (idxRegData * scale);
}

void m68000cpu::addressRegisterIndirectBaseDisplacement() {
    auto extWord = decodeExtensionWord((read(pc++) << 8) + read(pc++));
    uint32_t baseAddress = extWord.baseRegSuppress ? 0 : addressRegisters[opWord & EA_REG_MASK];
    int32_t idxAddress = 0;
    uint8_t scale = pow(2, extWord.scale);
    if (!extWord.indexSuppress) {
        auto idxRegisters = extWord.idxRegType == M68K_REG_TYPE_DATA ? dataRegisters : addressRegisters;
        idxAddress = idxRegisters[extWord.idxRegAddr];
    }
    int32_t displacement = 0;
    if (extWord.baseDisplacementSize == EXT_WORD_BD_SIZE_WORD) {
        displacement = (read(pc++) << 8) | read(pc++);
    }
    else if (extWord.baseDisplacementSize == EXT_WORD_BD_SIZE_LONG) {
        displacement = (read(pc++) << 24) | (read(pc++) << 16) | (read(pc++) << 8) | read(pc++);
    }
    address = baseAddress + displacement + (idxAddress * scale);
}

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


