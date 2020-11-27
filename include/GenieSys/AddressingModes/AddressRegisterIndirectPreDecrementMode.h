//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"

class AddressRegisterIndirectPreDecrementMode : public AddressingMode{
public:
    AddressRegisterIndirectPreDecrementMode(M68kCpu *cpu, Bus *bus);
    ~AddressRegisterIndirectPreDecrementMode() override = default;

    uint32_t getAddress() override;
    uint8_t getModeId() override;
};


