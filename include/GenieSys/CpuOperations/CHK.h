//
// Created by paul on 8/18/25.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys
{
    class CHK : public GenieSys::CpuOperation
    {
    private:
        const uint16_t baseOpWord = 0b0100000000000000;
        BitMask<uint16_t> dnMask = BitMask<uint16_t>(11, 3);
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(8, 2, 3, 3);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaAddrMask = BitMask<uint16_t>(2, 3);
    public:
        CHK(M68kCpu* cpu, Bus* bus);
        std::string disassemble(uint16_t opWord) override;
        uint8_t execute(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
    };
}