//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"

class ProgramCounterIndirectDisplacementMode : public AddressingMode {
public:
    ProgramCounterIndirectDisplacementMode(M68kCpu *cpu, Bus *bus);
    uint32_t getAddress() override;
    uint8_t getModeId() override;
};


