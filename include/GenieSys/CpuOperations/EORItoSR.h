//
// Created by paul.trampert on 12/25/2020.
//

#pragma once
#include "./CpuOperation.h"



class EORItoSR : public CpuOperation {
public:
    EORItoSR(GenieSys::M68kCpu* cpu, GenieSys::Bus* bus);
    std::vector<uint16_t> getOpcodes() override;
    uint8_t execute(uint16_t opWord) override;
    uint8_t getSpecificity() override;
    std::string disassemble(uint16_t opWord) override;
};