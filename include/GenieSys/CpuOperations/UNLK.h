//
// Created by paul on 12/11/21.
//

#pragma once

#include "CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class UNLK : public CpuOperation {
    private:
        uint16_t baseOpWord = 0b0100111001011000;
        BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);

    public:
        UNLK(M68kCpu * cpu, Bus * bus);

        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}