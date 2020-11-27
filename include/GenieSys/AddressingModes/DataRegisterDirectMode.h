//
// Created by paul.trampert on 11/27/2020.
//

#pragma once
#include "GenieSys/AddressingModes/AddressingMode.h"

class DataRegisterDirectMode : public AddressingMode {
public:
    DataRegisterDirectMode(M68kCpu *cpu, Bus *bus);
    ~DataRegisterDirectMode() override;
    uint32_t getAddress() override;
};


