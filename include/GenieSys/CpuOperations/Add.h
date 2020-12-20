//
// Created by paul.trampert on 12/6/2020.
//

#pragma once
#include "../AddressingModes/AddressingMode.h"
#include "./CpuOperation.h"

class Add : public CpuOperation {
private:
    void addBytes(uint8_t direction, uint8_t dataRegAddr, AddressingResult* eaResult);
    void addWords(uint8_t direction, uint8_t dataRegAddr, AddressingResult* eaResult);
    void addLongs(uint8_t direction, uint8_t dataRegAddr, AddressingResult* eaResult);
public:
    Add(M68kCpu* cpu, Bus* bus);
    void execute(uint16_t opWord) override;
    std::vector<uint16_t> getOpcodes() override;
    uint8_t getSpecificity() override;
    std::string disassemble(uint16_t opWord) override;
};