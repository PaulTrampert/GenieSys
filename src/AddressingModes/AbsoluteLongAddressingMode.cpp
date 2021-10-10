//
// Created by paul.trampert on 11/27/2020.
//

#include <vector>
#include <sstream>
#include "GenieSys/AddressingModes/AbsoluteLongAddressingMode.h"



GenieSys::AbsoluteLongAddressingMode::AbsoluteLongAddressingMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::AddressingMode(cpu, bus) {
    cycles = 12;
    longCycles = 16;
}

uint32_t GenieSys::AbsoluteLongAddressingMode::getAddress(uint8_t regAddr) {
    uint32_t address = bus->readLong(cpu->getPc());
    cpu->incrementPc(4);
    return address;
}

uint8_t GenieSys::AbsoluteLongAddressingMode::getModeId() {
    return MODE_ID;
}

std::string GenieSys::AbsoluteLongAddressingMode::disassemble(uint8_t regAddr, uint8_t size) {
    std::stringstream stream;
    stream << "(" << getAddress(regAddr) << ").L";
    return stream.str();
}

uint8_t GenieSys::AbsoluteLongAddressingMode::getMoveCycleKey() {
    return 8;
}
