//
// Created by paul on 6/20/24.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"
#include "../M68kCpu.h"
#include "../Bus.h"
#include <cstdint>

namespace GenieSys {
    class BRA : public CpuOperation {
    private:
        uint16_t baseOpWord = 0b0110000000000000;
        BitMask<uint16_t> displacementMask = BitMask<uint16_t>(7, 8);

    public:
        BRA(M68kCpu* cpu, Bus* bus);

        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
