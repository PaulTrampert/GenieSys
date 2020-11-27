//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AbsoluteLongAddressingMode.h"

AbsoluteLongAddressingMode::AbsoluteLongAddressingMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {

}

uint32_t AbsoluteLongAddressingMode::getAddress() {
    uint32_t address = bus->readLong(cpu->getPc());
    cpu->incrementPc(4);
    return address;
}
