//
// Created by pault on 10/24/2021.
//

#pragma once
#include "CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class SWAP : public CpuOperation {
    private:
        BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);
        uint16_t baseOpWord = 0b0100100001000000;

    public:
        SWAP(M68kCpu* cpu, Bus* bus);
        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}