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
    AddressingMode* mode = new ProgramCounterIndirectDisplacementMode(cpu, bus);
    subModes[mode->getModeId()] = mode;
    mode = new ProgramCounterIndirectWithIndexMode(cpu, bus);
    subModes[mode->getModeId()] = mode;
    mode = new AbsoluteShortAddressingMode(cpu, bus);
    subModes[mode->getModeId()] = mode;
    mode = new AbsoluteLongAddressingMode(cpu, bus);
    subModes[mode->getModeId()] = mode;
    mode = new ImmediateDataMode(cpu, bus);
    subModes[mode->getModeId()] = mode;
}

uint32_t ProgramCounterAddressingMode::getAddress() {
    uint8_t submodeId = cpu->getCurrentOpWord() & EA_REG_MASK;
    return subModes[submodeId]->getAddress();
}

uint8_t ProgramCounterAddressingMode::getModeId() {
    return 0b111u;
}

ProgramCounterAddressingMode::~ProgramCounterAddressingMode() {
    for (auto & subMode : subModes) {
        delete subMode;
    }
}

void ProgramCounterAddressingMode::setBus(Bus *b) {
    this->bus = b;
    for (auto & subMode : subModes) {
        if (subMode != nullptr) {
            subMode->setBus(b);
        }
    }
}
