//
// Created by pault on 10/4/2021.
//

#pragma once
#include "CpuOperation.h"


class CLR : public CpuOperation {
private:
    GenieSys::BitMask<uint16_t> sizeMask = GenieSys::BitMask<uint16_t>(7, 2, 0, 2);
    GenieSys::BitMask<uint16_t> eaModeMask = GenieSys::BitMask<uint16_t>(5, 3);
    GenieSys::BitMask<uint16_t> eaRegMask = GenieSys::BitMask<uint16_t>(2, 3);

public:
    CLR(M68kCpu* cpu, Bus* bus);

    uint8_t getSpecificity() override;
    std::vector<uint16_t> getOpcodes() override;
    uint8_t execute(uint16_t opWord) override;
    std::string disassemble(uint16_t opWord) override;
};