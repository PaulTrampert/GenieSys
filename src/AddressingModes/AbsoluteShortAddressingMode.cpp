//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/signExtend.h>
#include <sstream>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include "GenieSys/AddressingModes/AbsoluteShortAddressingMode.h"



GenieSys::AbsoluteShortAddressingMode::AbsoluteShortAddressingMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::AddressingMode(cpu, bus) {
    cycles = 8;
    longCycles = 12;
}

uint32_t GenieSys::AbsoluteShortAddressingMode::getAddress(uint8_t regAddr) {
    uint32_t address = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return address;
}

uint8_t GenieSys::AbsoluteShortAddressingMode::getModeId() {
    return MODE_ID;
}

std::string GenieSys::AbsoluteShortAddressingMode::disassemble(uint8_t regAddr, uint8_t size) {
    std::stringstream stream;
    stream << "(" << getAddress(regAddr) << ").W";
    return stream.str();
}
