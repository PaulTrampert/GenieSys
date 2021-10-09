//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include <cstdint>
#include "AddressingMode.h"


class AddressRegisterDirectResult : public AddressingResult {
public:
    AddressRegisterDirectResult(GenieSys::M68kCpu* cpu, Bus* bus, uint32_t address, std::vector<uint8_t> data);
    void write(std::vector<uint8_t> data) override;
    void write(uint8_t data) override;
    void write(uint16_t data) override;
    void write(uint32_t data) override;
};

class AddressRegisterDirectMode : public AddressingMode {
public:
    static const uint8_t MODE_ID = 0b001u;

    AddressRegisterDirectMode(GenieSys::M68kCpu *cpu, Bus *bus);

    uint32_t getAddress(uint8_t regAddr) override;
    uint8_t getModeId() override;
    std::unique_ptr<AddressingResult> getData(uint8_t regAddr, uint8_t size) override;
    std::string disassemble(uint8_t regAddr, uint8_t size) override;
};


