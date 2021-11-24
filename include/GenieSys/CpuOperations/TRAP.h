//
// Created by paul on 11/23/21.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class TRAP : public CpuOperation {
    private:
        uint16_t baseOpCode = 0b0100111001000000;
        BitMask<uint16_t> vectorMask = BitMask<uint16_t>(3, 4);

    public:
        TRAP(M68kCpu* cpu, Bus* bus);
        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}