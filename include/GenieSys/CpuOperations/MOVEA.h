//
// Created by pault on 9/18/2021.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class MOVEA : public GenieSys::CpuOperation {
    private:
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(13, 2, 2, 3);
        BitMask<uint16_t> destMask = BitMask<uint16_t>(11, 3);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaAddrMask = BitMask<uint16_t>(2, 3);

    public:
        MOVEA(M68kCpu* cpu, Bus* bus);
        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
