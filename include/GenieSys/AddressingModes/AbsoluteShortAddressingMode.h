//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"

class AbsoluteShortAddressingMode : public AddressingMode {
public:
    AbsoluteShortAddressingMode(M68kCpu *cpu, Bus *bus);
    ~AbsoluteShortAddressingMode() override = default;

    uint32_t getAddress() override;
};


