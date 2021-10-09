//
// Created by paul.trampert on 12/25/2020.
//

#pragma once

#include "CpuOperation.h"



class EORI : public CpuOperation {
private:
    uint16_t BASE_OPCODE = 0b0000101000000000;
    GenieSys::BitMask<uint16_t> sizeMask = GenieSys::BitMask<uint16_t>(7, 2, 0, 2);
    GenieSys::BitMask<uint16_t> eaModeMask = GenieSys::BitMask<uint16_t>(5, 3);
    GenieSys::BitMask<uint16_t> eaRegMask = GenieSys::BitMask<uint16_t>(2, 3);

public:
    EORI(GenieSys::M68kCpu* cpu, GenieSys::Bus* bus);
    std::string disassemble(uint16_t opWord) override;
    uint8_t getSpecificity() override;
    uint8_t execute(uint16_t opWord) override;
    std::vector<uint16_t> getOpcodes() override;
};