//
// Created by paul.trampert on 12/16/2020.
//

#pragma once
#include "./CpuOperation.h"



class ANDItoCCR : public CpuOperation {
public:
    ANDItoCCR(GenieSys::M68kCpu* cpu, GenieSys::Bus* bus);
    uint8_t getSpecificity() override;
    uint8_t execute(uint16_t opWord) override;
    std::vector<uint16_t> getOpcodes() override;
    std::string disassemble(uint16_t opWord) override;
};