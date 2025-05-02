//
// Created by paul on 5/28/24.
//

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "../BitMask.h"
#include "./CpuOperation.h"

namespace GenieSys {
    class Bcc : public CpuOperation {
    private:
        const uint16_t baseOpcode = 0b0110000000000000;
        BitMask<uint16_t> conditionMask = BitMask<uint16_t>(11, 4, 0b0010, 0b1111);
        BitMask<uint16_t> displacementMask = BitMask<uint16_t>(7, 8);

    public:
        Bcc(M68kCpu* cpu, Bus* bus);
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
