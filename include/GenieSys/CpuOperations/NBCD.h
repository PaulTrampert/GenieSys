//
// Created by pault on 10/22/2021.
//

#pragma once
#include "CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class NBCD : public CpuOperation {
    private:
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);
        BitMask<uint8_t> tensMask = BitMask<uint8_t>(7, 4);
        BitMask<uint8_t> onesMask = BitMask<uint8_t>(3, 4);
        uint16_t baseOpcode = 0b0100100000000000u;

    public:
        NBCD(M68kCpu* cpu, Bus* bus);
        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}