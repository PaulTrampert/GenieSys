//
// Created by copilot
//

#pragma once
#include "CpuOperation.h"
#include "../BitMask.h"
#include "../enums.h"
#include "../M68kCpu.h"

namespace GenieSys {
    class AddressingResult;

    /**
     * Documented on page 4-113 of docs/68KPM.pdf
     * Handles both ROL (rotate left) and ROR (rotate right) for register and memory forms.
     *
     * Register form: 1110 ccc d ss i 11 rrr
     *   ccc = rotation count (immediate) or source data register number
     *   d   = direction (1=left, 0=right)
     *   ss  = size (00=byte, 01=word, 10=long)
     *   i   = 0 immediate count, 1 register count
     *   rrr = destination data register
     *
     * Memory form: 1110 011 d 11 eaMode eaReg
     *   d = direction (1=left, 0=right), word size only, 1-bit rotate
     */
    class ROd : public GenieSys::CpuOperation {
    private:
        // Register form masks
        BitMask<uint16_t> countRegMask = BitMask<uint16_t>(11, 3);
        BitMask<uint16_t> directionMask = BitMask<uint16_t>(8, 1);
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(7, 2, 0, 2); // size 0-2 (byte/word/long)
        BitMask<uint16_t> irMask = BitMask<uint16_t>(5, 1);
        BitMask<uint16_t> destRegMask = BitMask<uint16_t>(2, 3);

        // Memory form masks (bits 5-3 = EA mode, bits 2-0 = EA reg)
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

        template<class T>
        T doRotateLeft(T value, uint32_t effectiveCount) {
            constexpr uint8_t bits = sizeof(T) * 8;
            uint8_t n = effectiveCount % bits;
            if (n == 0) return value;
            return static_cast<T>((value << n) | (value >> (bits - n)));
        }

        template<class T>
        T doRotateRight(T value, uint32_t effectiveCount) {
            constexpr uint8_t bits = sizeof(T) * 8;
            uint8_t n = effectiveCount % bits;
            if (n == 0) return value;
            return static_cast<T>((value >> n) | (value << (bits - n)));
        }

        // For ROL: carry = LSB of result (the bit that wrapped from MSB) when count > 0
        template<class T>
        bool carryROL(T result, uint32_t effectiveCount) {
            return effectiveCount > 0 && (result & 1) != 0;
        }

        // For ROR: carry = MSB of result (the bit that wrapped from LSB) when count > 0
        template<class T>
        bool carryROR(T result, uint32_t effectiveCount) {
            constexpr uint8_t bits = sizeof(T) * 8;
            return effectiveCount > 0 && (result >> (bits - 1)) != 0;
        }

        template<class T, class S>
        uint8_t computeCcr(T result, bool carry) {
            uint8_t ccr = cpu->getCcrFlags() & CCR_EXTEND; // X not affected
            if (carry) ccr |= CCR_CARRY;
            if (static_cast<S>(result) < 0) ccr |= CCR_NEGATIVE;
            if (result == 0) ccr |= CCR_ZERO;
            // V always cleared
            return ccr;
        }

        uint8_t executeRegister(uint16_t opWord, uint8_t size);
        uint8_t executeMemory(uint16_t opWord);

    public:
        ROd(M68kCpu* cpu, Bus* bus);

        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
