//
// Created by paul.trampert on 12/25/2020.
//

#pragma once

#include "CpuOperation.h"


class ANDI : public CpuOperation {
private:
    GenieSys::BitMask<uint16_t> sizeMask = GenieSys::BitMask<uint16_t>(7, 2, 0, 2);
    GenieSys::BitMask<uint16_t> eaModeMask = GenieSys::BitMask<uint16_t>(5, 3);
    GenieSys::BitMask<uint16_t> eaRegMask = GenieSys::BitMask<uint16_t>(2, 3);

public:
    ANDI(M68kCpu* cpu, Bus* bus);
    std::string disassemble(uint16_t opWord) override;
    uint8_t getSpecificity() override;
    uint8_t execute(uint16_t opWord) override;
    std::vector<uint16_t> getOpcodes() override;
};