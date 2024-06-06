//
// Created by paul on 6/5/24.
//

#pragma once
#include "./CpuOperation.h"
#include "GenieSys/BitMask.h"
#include <cstdint>
#include <vector>

namespace GenieSys {
    class BSR : public GenieSys::CpuOperation {
    private:
        uint16_t baseOpWord = 0b0110000100000000;
        BitMask<uint16_t> displacementMask = BitMask<uint16_t>(7, 8);

    public:
        BSR(M68kCpu* cpu, Bus* bus);
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
    };
}