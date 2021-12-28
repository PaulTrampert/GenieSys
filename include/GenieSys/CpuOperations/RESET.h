//
// Created by pault on 12/28/2021.
//

#pragma once

#include "./CpuOperation.h"

namespace GenieSys {
    class RESET : public CpuOperation {
    private:
        uint16_t opWord = 0b0100111001110000;

    public:
        RESET(M68kCpu * cpu, Bus * bus);
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}