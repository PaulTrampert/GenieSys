//
// Created by paul on 11/18/21.
//
#pragma once

#include "../enums.h"
#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class TST : public CpuOperation {
    private:
        uint16_t baseOpWord = 0b0100101000000000;
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(7, 2, 0, 2);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

        template <class U, class S> uint8_t exec(U data, uint8_t ccr) {
            if (data == 0) {
                ccr |= CCR_ZERO;
            }
            if ((S)data < 0) {
                ccr |= CCR_NEGATIVE;
            }
            return ccr;
        }

    public:
        TST(M68kCpu* cpu, Bus* bus);
        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
