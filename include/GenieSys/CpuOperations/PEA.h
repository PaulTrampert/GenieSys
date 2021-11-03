//
// Created by pault on 10/25/2021.
//

#pragma once

#include "CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class PEA : public CpuOperation {
    private:
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);
        uint16_t opWordBase = 0b0100100001000000;

    public:
        PEA(M68kCpu* cpu, Bus* bus);
        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}