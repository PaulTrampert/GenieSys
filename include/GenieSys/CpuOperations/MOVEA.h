//
// Created by pault on 9/18/2021.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"

class MOVEA : public GenieSys::CpuOperation {
private:
    GenieSys::BitMask<uint16_t> sizeMask = GenieSys::BitMask<uint16_t>(13, 2, 2, 3);
    GenieSys::BitMask<uint16_t> destMask = GenieSys::BitMask<uint16_t>(11, 3);
    GenieSys::BitMask<uint16_t> eaModeMask = GenieSys::BitMask<uint16_t>(5, 3);
    GenieSys::BitMask<uint16_t> eaAddrMask = GenieSys::BitMask<uint16_t>(2, 3);

public:
    MOVEA(GenieSys::M68kCpu* cpu, GenieSys::Bus* bus);
    uint8_t getSpecificity() override;
    std::vector<uint16_t> getOpcodes() override;
    uint8_t execute(uint16_t opWord) override;
    std::string disassemble(uint16_t opWord) override;
};
