//
// Created by paul on 2/28/22.
//
#pragma once
#include "./CpuOperation.h"

namespace GenieSys {
    class RTE : public CpuOperation {
    private:
        uint16_t opWord = 0b0100111001110011;

    public:
        RTE(M68kCpu *cpu, Bus *bus);

        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}