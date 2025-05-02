//
// Created by paul on 5/2/25.
//

#pragma once
#include <string>
#include <vector>
#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class DBcc : public GenieSys::CpuOperation {
    private:
        const uint16_t BASE_OPCODE = 0b0101000011001000;
        BitMask<uint16_t> conditionMask = BitMask<uint16_t>(11, 4, 0, 2);
        BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);

    public:
        DBcc(M68kCpu* cpu, Bus* bus);
        std::string disassemble(uint16_t opWord) override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
    };
}
