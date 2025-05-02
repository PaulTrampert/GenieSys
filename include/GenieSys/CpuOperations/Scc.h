//
// Created by paul on 5/2/25.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class Scc : public GenieSys::CpuOperation {
    private:
        const uint16_t BASE_OPCODE = 0b0101000011000000;
        BitMask<uint16_t> conditionMask = BitMask<uint16_t>(11, 4);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    public:
        Scc(M68kCpu* cpu, Bus* bus);
        std::string disassemble(uint16_t opWord) override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
    };
}