//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"

class ProgramCounterIndirectWithIndexMode : public AddressingMode {
public:
    ProgramCounterIndirectWithIndexMode(M68kCpu *cpu, Bus *bus);
    ~ProgramCounterIndirectWithIndexMode() override = default;
    uint32_t getAddress() override;
    uint8_t getModeId() override;
};


