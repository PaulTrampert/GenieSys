//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/signExtend.h>
#include <sstream>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include "GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h"



GenieSys::ProgramCounterIndirectDisplacementMode::ProgramCounterIndirectDisplacementMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus)
    : GenieSys::AddressingMode(cpu, bus) {
    cycles = 8;
    longCycles = 12;
}

uint32_t GenieSys::ProgramCounterIndirectDisplacementMode::getAddress(uint8_t regAddr) {
    uint32_t address = cpu->getPc() + GenieSys::signExtend<int32_t>((int32_t)bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return address;
}

uint8_t GenieSys::ProgramCounterIndirectDisplacementMode::getModeId() {
    return MODE_ID;
}

std::string GenieSys::ProgramCounterIndirectDisplacementMode::disassemble(uint8_t regAddr, uint8_t size) {
    auto displacement = GenieSys::signExtend<int32_t>((int32_t)bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    std::stringstream stream;
    stream << "(#" << displacement << ",PC)";
    return stream.str();
}

uint8_t GenieSys::ProgramCounterIndirectDisplacementMode::getMoveCycleKey() {
    return 9;
}
