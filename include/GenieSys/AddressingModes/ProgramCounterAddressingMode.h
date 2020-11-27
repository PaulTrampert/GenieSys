//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"

class ProgramCounterAddressingMode : public AddressingMode {
private:
    std::array<AddressingMode*, 8> subModes;
public:
    ProgramCounterAddressingMode(M68kCpu *cpu, Bus *bus);
    ~ProgramCounterAddressingMode() override;
    uint32_t getAddress() override;
    uint8_t getModeId() override;
    void setBus(Bus *b) override;
};


