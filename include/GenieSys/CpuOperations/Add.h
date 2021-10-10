//
// Created by paul.trampert on 12/6/2020.
//

#pragma once
#include "../AddressingModes/AddressingMode.h"
#include "./CpuOperation.h"


class Add : public GenieSys::CpuOperation {
private:
    void addBytes(uint8_t direction, uint8_t dataRegAddr, GenieSys::AddressingResult* eaResult);
    void addWords(uint8_t direction, uint8_t dataRegAddr, GenieSys::AddressingResult* eaResult);
    void addLongs(uint8_t direction, uint8_t dataRegAddr, GenieSys::AddressingResult* eaResult);
public:
    Add(GenieSys::M68kCpu* cpu, GenieSys::Bus* bus);
    uint8_t execute(uint16_t opWord) override;
    std::vector<uint16_t> getOpcodes() override;
    uint8_t getSpecificity() override;
    std::string disassemble(uint16_t opWord) override;
};