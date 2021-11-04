//
// Created by pault on 11/3/2021.
//

#pragma once
#include "./CpuOperation.h"

namespace GenieSys {
    class ILLEGAL : public CpuOperation {
    private:
        uint16_t opWord = 0b0100101011111100;

    public:
        ILLEGAL(M68kCpu* cpu, Bus* bus);
        std::string disassemble(uint16_t opWord) override;
        uint8_t execute(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
    };
}