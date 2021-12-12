//
// Created by paul on 12/10/21.
//

#pragma once

#include "CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class LINK : public CpuOperation {
    private:
        uint16_t baseOpWord = 0b0100111001010000;
        BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);

    public:
        LINK(M68kCpu * cpu, Bus * bus);
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}