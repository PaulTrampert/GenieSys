//
// Created by paul.trampert on 12/25/2020.
//

#pragma once

#include "CpuOperation.h"
#include "../BitMask.h"


namespace GenieSys {
    class ORI : public GenieSys::CpuOperation {
    private:
        const uint16_t BASE_OPCODE = 0b0000000000000000;
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(7, 2, 0, 2);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    public:
        ORI(M68kCpu* cpu, Bus* bus);
        std::string disassemble(uint16_t opWord) override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
    };
}