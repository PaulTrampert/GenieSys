//
// Created by pault on 9/26/2021.
//

#pragma once

#include "CpuOperation.h"
#include "../BitMask.h"


namespace GenieSys {
    class MOVEtoCCR : public GenieSys::CpuOperation {
    private:
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    public:
        MOVEtoCCR(M68kCpu* cpu, Bus* bus);
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
        uint8_t getSpecificity() override;
    };
}
