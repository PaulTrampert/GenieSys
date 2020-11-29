//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"

class AddressRegisterIndirectWithIndexMode : public AddressingMode {
public:
    static const uint8_t MODE_ID = 0b110u;

    AddressRegisterIndirectWithIndexMode(M68kCpu *cpu, Bus *bus);

    uint32_t getAddress(uint8_t regAddr) override;
    uint8_t getModeId() override;
};


