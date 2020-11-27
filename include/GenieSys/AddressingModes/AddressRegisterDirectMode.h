//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include <cstdint>
#include "AddressingMode.h"

class AddressRegisterDirectMode : public AddressingMode {
public:
    AddressRegisterDirectMode(M68kCpu *cpu, Bus *bus);

    uint32_t getAddress() override;
    uint8_t getModeId() override;
};


