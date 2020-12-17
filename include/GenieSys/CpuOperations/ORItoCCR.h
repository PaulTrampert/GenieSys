//
// Created by paul.trampert on 12/16/2020.
//

#pragma once
#include "./CpuOperation.h"

class ORItoCCR : public CpuOperation {
public:
    ORItoCCR(M68kCpu* cpu, Bus* bus);
    uint8_t getSpecificity() override;
    void execute(uint16_t opWord) override;
    std::vector<uint16_t> getOpcodes() override;
};