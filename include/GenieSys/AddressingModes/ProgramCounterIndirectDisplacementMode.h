//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"



class ProgramCounterIndirectDisplacementMode : public AddressingMode {
public:
    static const uint8_t MODE_ID = 0b010u;

    ProgramCounterIndirectDisplacementMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus);
    uint32_t getAddress(uint8_t regAddr) override;
    uint8_t getModeId() override;
    std::string disassemble(uint8_t regAddr, uint8_t size) override;
    uint8_t getMoveCycleKey() override;
};


