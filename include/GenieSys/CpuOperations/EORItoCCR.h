//
// Created by paul.trampert on 12/16/2020.
//

#pragma once
#include "./CpuOperation.h"

class EORItoCCR : public CpuOperation {
private:
    const uint16_t BASE_OPCODE = 0b0000101000111100;

public:
    EORItoCCR(M68kCpu* cpu, Bus* bus);
    uint8_t getSpecificity() override;
    uint8_t execute(uint16_t opWord) override;
    std::vector<uint16_t> getOpcodes() override;
    std::string disassemble(uint16_t opWord) override;
};