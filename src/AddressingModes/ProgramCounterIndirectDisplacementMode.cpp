//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/signExtend.h>
#include "GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h"

ProgramCounterIndirectDisplacementMode::ProgramCounterIndirectDisplacementMode(M68kCpu *cpu, Bus *bus)
    : AddressingMode(cpu, bus) {

}

uint32_t ProgramCounterIndirectDisplacementMode::getAddress() {
    uint32_t address = cpu->getPc() + signExtend<int32_t>((int32_t)bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return address;
}

uint8_t ProgramCounterIndirectDisplacementMode::getModeId() {
    return 0b010u;
}
