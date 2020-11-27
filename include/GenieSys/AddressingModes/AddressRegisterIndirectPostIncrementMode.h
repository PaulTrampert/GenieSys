//
// Created by paul.trampert on 11/27/2020.
//

#pragma once

#include "AddressingMode.h"

class AddressRegisterIndirectPostIncrementMode : public AddressingMode{
public:
    AddressRegisterIndirectPostIncrementMode(M68kCpu *cpu, Bus *bus);
    ~AddressRegisterIndirectPostIncrementMode() override = default;

    uint32_t getAddress() override;
    uint8_t getModeId() override;
};


