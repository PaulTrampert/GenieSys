//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectWithIndexMode.h>
#include <GenieSys/AddressingModes/AbsoluteShortAddressingMode.h>
#include <GenieSys/AddressingModes/AbsoluteLongAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include "GenieSys/AddressingModes/ProgramCounterAddressingMode.h"

ProgramCounterAddressingMode::ProgramCounterAddressingMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {
    for (auto & subMode : subModes) {
        subMode = nullptr;
    }
    subModes[ProgramCounterIndirectDisplacementMode::MODE_ID] = std::unique_ptr<AddressingMode>(new ProgramCounterIndirectDisplacementMode(cpu, bus));
    subModes[ProgramCounterIndirectWithIndexMode::MODE_ID] = std::unique_ptr<AddressingMode>(new ProgramCounterIndirectWithIndexMode(cpu, bus));
    subModes[AbsoluteShortAddressingMode::MODE_ID] = std::unique_ptr<AddressingMode>(new AbsoluteShortAddressingMode(cpu, bus));
    subModes[AbsoluteLongAddressingMode::MODE_ID] = std::unique_ptr<AddressingMode>(new AbsoluteLongAddressingMode(cpu, bus));
    subModes[ImmediateDataMode::MODE_ID] = std::unique_ptr<AddressingMode>(new ImmediateDataMode(cpu, bus));
}

uint32_t ProgramCounterAddressingMode::getAddress(uint8_t regAddr) {
    uint8_t submodeId = regAddr;
    AddressingMode* subMode = subModes[submodeId].get();
    if (subMode == nullptr) return 0;
    return subMode->getAddress(regAddr);
}

uint8_t ProgramCounterAddressingMode::getModeId() {
    return 0b111u;
}

ProgramCounterAddressingMode::~ProgramCounterAddressingMode() = default;

void ProgramCounterAddressingMode::setBus(Bus *b) {
    this->bus = b;
    for (auto & subMode : subModes) {
        if (subMode != nullptr) {
            subMode->setBus(b);
        }
    }
}

std::unique_ptr<AddressingResult> ProgramCounterAddressingMode::getData(uint8_t regAddr, uint8_t size) {
    uint8_t submodeId = regAddr;
    AddressingMode* subMode = subModes[submodeId].get();
    if (subMode == nullptr) return std::make_unique<AddressingResult>(cpu, bus, 0, std::vector<uint8_t>(0), 0, 0);
    return subMode->getData(regAddr, size);
}

std::string ProgramCounterAddressingMode::disassemble(uint8_t regAddr, uint8_t size) {
    uint8_t submodeId = regAddr;
    AddressingMode* subMode = subModes[submodeId].get();
    if (subMode == nullptr) return std::string();
    return subMode->disassemble(regAddr, size);
}
