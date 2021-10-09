//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include <memory>
#include "AddressingMode.h"


class ProgramCounterAddressingMode : public AddressingMode {
private:
    std::array<std::unique_ptr<AddressingMode>, 8> subModes;
public:
    static const uint8_t MODE_ID = 0b111u;

    ProgramCounterAddressingMode(GenieSys::M68kCpu *cpu, Bus *bus);
    ~ProgramCounterAddressingMode() override;
    uint32_t getAddress(uint8_t regAddr) override;
    uint8_t getModeId() override;
    void setBus(Bus *b) override;
    std::unique_ptr<AddressingResult> getData(uint8_t regAddr, uint8_t size) override;
    std::string disassemble(uint8_t regAddr, uint8_t size) override;
};


