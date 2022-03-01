//
// Created by paul on 2/28/22.
//

#pragma once
#include "./CpuOperation.h"

namespace GenieSys {
    class RTR : public CpuOperation {
    private:
        uint16_t opWord = 0b0100111001110111;

    public:
        RTR(M68kCpu *cpu, Bus *bus);

        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t _) override;
        std::string disassemble(uint16_t _) override;
    };
}