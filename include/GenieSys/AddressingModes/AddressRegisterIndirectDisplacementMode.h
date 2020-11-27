//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"

class AddressRegisterIndirectDisplacementMode : public AddressingMode {
public:
    AddressRegisterIndirectDisplacementMode(M68kCpu *cpu, Bus *bus);
    ~AddressRegisterIndirectDisplacementMode() override = default;
    uint32_t getAddress() override;
};


