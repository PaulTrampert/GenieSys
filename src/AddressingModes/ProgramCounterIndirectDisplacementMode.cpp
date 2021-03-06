//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/signExtend.h>
#include <sstream>
#include "GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h"

ProgramCounterIndirectDisplacementMode::ProgramCounterIndirectDisplacementMode(M68kCpu *cpu, Bus *bus)
    : AddressingMode(cpu, bus) {
    cycles = 8;
    longCycles = 12;
}

uint32_t ProgramCounterIndirectDisplacementMode::getAddress(uint8_t regAddr) {
    uint32_t address = cpu->getPc() + signExtend<int32_t>((int32_t)bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return address;
}

uint8_t ProgramCounterIndirectDisplacementMode::getModeId() {
    return MODE_ID;
}

std::string ProgramCounterIndirectDisplacementMode::disassemble(uint8_t regAddr, uint8_t size) {
    auto displacement = signExtend<int32_t>((int32_t)bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    std::stringstream stream;
    stream << "(#" << displacement << ",PC)";
    return stream.str();
}
