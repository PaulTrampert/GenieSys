//
// Created by paul.trampert on 4/4/2021.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"



class BSET : public CpuOperation {
private:
    GenieSys::BitMask<uint16_t> DnMask = GenieSys::BitMask<uint16_t>(11, 3);
    GenieSys::BitMask<uint16_t> EaModeMask = GenieSys::BitMask<uint16_t>(5, 3);
    GenieSys::BitMask<uint16_t> EaAddrMask = GenieSys::BitMask<uint16_t>(2, 3);
    GenieSys::BitMask<uint16_t> ImmMask = GenieSys::BitMask<uint16_t>(8, 1);

public:
    BSET(GenieSys::M68kCpu* cpu, Bus* bus);
    std::vector<uint16_t> getOpcodes() override;
    uint8_t getSpecificity() override;
    uint8_t execute(uint16_t opWord) override;
    std::string disassemble(uint16_t opWord) override;
};