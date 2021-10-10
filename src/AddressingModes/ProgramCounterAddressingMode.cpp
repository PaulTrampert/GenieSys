//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectWithIndexMode.h>
#include <GenieSys/AddressingModes/AbsoluteShortAddressingMode.h>
#include <GenieSys/AddressingModes/AbsoluteLongAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include "GenieSys/AddressingModes/ProgramCounterAddressingMode.h"



GenieSys::ProgramCounterAddressingMode::ProgramCounterAddressingMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : AddressingMode(cpu, bus) {
    for (auto & subMode : subModes) {
        subMode = nullptr;
    }
    subModes[ProgramCounterIndirectDisplacementMode::MODE_ID] = std::unique_ptr<AddressingMode>(new ProgramCounterIndirectDisplacementMode(cpu, bus));
    subModes[ProgramCounterIndirectWithIndexMode::MODE_ID] = std::unique_ptr<AddressingMode>(new ProgramCounterIndirectWithIndexMode(cpu, bus));
    subModes[GenieSys::AbsoluteShortAddressingMode::MODE_ID] = std::unique_ptr<AddressingMode>(new GenieSys::AbsoluteShortAddressingMode(cpu, bus));
    subModes[GenieSys::AbsoluteLongAddressingMode::MODE_ID] = std::unique_ptr<AddressingMode>(new GenieSys::AbsoluteLongAddressingMode(cpu, bus));
    subModes[GenieSys::ImmediateDataMode::MODE_ID] = std::unique_ptr<AddressingMode>(new GenieSys::ImmediateDataMode(cpu, bus));
}

uint32_t GenieSys::ProgramCounterAddressingMode::getAddress(uint8_t regAddr) {
    uint8_t submodeId = regAddr;
    AddressingMode* subMode = subModes[submodeId].get();
    if (subMode == nullptr) return 0;
    return subMode->getAddress(regAddr);
}

uint8_t GenieSys::ProgramCounterAddressingMode::getModeId() {
    return 0b111u;
}

GenieSys::ProgramCounterAddressingMode::~ProgramCounterAddressingMode() = default;

void GenieSys::ProgramCounterAddressingMode::setBus(GenieSys::Bus *b) {
    this->bus = b;
    for (auto & subMode : subModes) {
        if (subMode != nullptr) {
            subMode->setBus(b);
        }
    }
}

std::unique_ptr<GenieSys::AddressingResult> GenieSys::ProgramCounterAddressingMode::getData(uint8_t regAddr, uint8_t size) {
    uint8_t submodeId = regAddr;
    AddressingMode* subMode = subModes[submodeId].get();
    if (subMode == nullptr) return std::make_unique<GenieSys::AddressingResult>(cpu, bus, 0, std::vector<uint8_t>(0), 0, 0);
    return subMode->getData(regAddr, size);
}

std::string GenieSys::ProgramCounterAddressingMode::disassemble(uint8_t regAddr, uint8_t size) {
    uint8_t submodeId = regAddr;
    AddressingMode* subMode = subModes[submodeId].get();
    if (subMode == nullptr) return {};
    return subMode->disassemble(regAddr, size);
}
