//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"

class AbsoluteLongAddressingMode : public AddressingMode {
public:
    AbsoluteLongAddressingMode(M68kCpu *cpu, Bus *bus);
    ~AbsoluteLongAddressingMode() override = default;

    uint32_t getAddress() override;
    uint8_t getModeId() override;
};


