//
// Created by paul.trampert on 12/16/2020.
//

#pragma once
#include "./CpuOperation.h"

class Nop : public CpuOperation {
public:
    Nop(M68kCpu* cpu, Bus* bus);
    std::vector<uint16_t> getOpcodes() override;
    void execute(uint16_t opWord) override;
    uint8_t getSpecificity() override;
};