//
// Created by pault on 9/26/2021.
//

#pragma once

#include "CpuOperation.h"
#include "../BitMask.h"


class MOVEtoCCR : public GenieSys::CpuOperation {
private:
    GenieSys::BitMask<uint16_t> eaModeMask = GenieSys::BitMask<uint16_t>(5, 3);
    GenieSys::BitMask<uint16_t> eaRegMask = GenieSys::BitMask<uint16_t>(2, 3);

public:
    MOVEtoCCR(GenieSys::M68kCpu* cpu, GenieSys::Bus* bus);
    std::vector<uint16_t> getOpcodes() override;
    uint8_t execute(uint16_t opWord) override;
    std::string disassemble(uint16_t opWord) override;
    uint8_t getSpecificity() override;
};
