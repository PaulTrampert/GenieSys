//
// Created by paul.trampert on 4/4/2021.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"

class BCLR : public CpuOperation {
private:
    BitMask<uint16_t> DnMask = BitMask<uint16_t>(11, 3);
    BitMask<uint16_t> EaModeMask = BitMask<uint16_t>(5, 3);
    BitMask<uint16_t> EaAddrMask = BitMask<uint16_t>(2, 3);
    BitMask<uint16_t> ImmMask = BitMask<uint16_t>(8, 1);

public:
    BCLR(M68kCpu* cpu, Bus* bus);
    std::vector<uint16_t> getOpcodes() override;
    uint8_t getSpecificity() override;
    uint8_t execute(uint16_t opWord) override;
    std::string disassemble(uint16_t opWord) override;
};