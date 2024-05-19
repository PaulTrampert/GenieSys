//
// Created by pault on 5/3/2022.
//

#pragma once

#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class MOVEM : public CpuOperation {
    private:
        uint16_t baseOpWord = 0b0100100010000000;
        BitMask<uint16_t> dirMask = BitMask<uint16_t>(10, 1);
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(6, 1);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    public:
        MOVEM(M68kCpu *cpu, Bus *bus);

        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}