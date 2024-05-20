//
// Created by paul on 5/20/24.
//
#pragma once
#include <cstdint>
#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class MOVEQ : public GenieSys::CpuOperation {
    private:
        uint16_t baseOpWord = 0b0111000000000000;
        BitMask<uint16_t> dataMask = BitMask<uint16_t>(7, 8);
        BitMask<uint16_t> destMask = BitMask<uint16_t>(11, 3);

    public:
        MOVEQ(M68kCpu *cpu, Bus *bus);
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
