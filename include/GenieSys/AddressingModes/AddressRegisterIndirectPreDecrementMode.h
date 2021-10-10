//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"



class AddressRegisterIndirectPreDecrementMode : public GenieSys::AddressingMode{
public:
    static const uint8_t MODE_ID = 0b100u;

    AddressRegisterIndirectPreDecrementMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus);

    uint32_t getAddress(uint8_t regAddr) override;
    uint8_t getModeId() override;
    std::unique_ptr<GenieSys::AddressingResult> getData(uint8_t regAddr, uint8_t size) override;
    std::string disassemble(uint8_t regAddr, uint8_t size) override;
};


