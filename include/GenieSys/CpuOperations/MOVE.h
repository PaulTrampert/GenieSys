//
// Created by pault on 9/19/2021.
//

#pragma once


#include "CpuOperation.h"


class MOVE : public CpuOperation{
private:
    GenieSys::BitMask<uint16_t> sizeMask = GenieSys::BitMask<uint16_t>(13, 2, 1, 3);
    GenieSys::BitMask<uint16_t> destEaModeMask = GenieSys::BitMask<uint16_t>(8, 3);
    GenieSys::BitMask<uint16_t> destEaRegMask = GenieSys::BitMask<uint16_t>(11, 3);
    GenieSys::BitMask<uint16_t> srcEaRegMask = GenieSys::BitMask<uint16_t>(2, 3);
    GenieSys::BitMask<uint16_t> srcEaModeMask = GenieSys::BitMask<uint16_t>(5, 3);

public:
    MOVE(M68kCpu* cpu, Bus* bus);
    std::vector<uint16_t> getOpcodes() override;
    uint8_t getSpecificity() override;
    uint8_t execute(uint16_t opWord) override;
    std::string disassemble(uint16_t opWord) override;
};
