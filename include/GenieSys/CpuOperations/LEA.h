//
// Created by copilot on 11/24/2024.
//

#pragma once

#include "CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class LEA : public CpuOperation {
    private:
        BitMask<uint16_t> destRegMask = BitMask<uint16_t>(11, 3);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);
        uint16_t opWordBase = 0b0100000111000000;

        uint8_t getLeaCycles(uint8_t eaModeId, uint8_t eaReg);

    public:
        LEA(M68kCpu* cpu, Bus* bus);
        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
