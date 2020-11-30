//
// Created by paul.trampert on 11/27/2020.
//

#include <vector>
#include "GenieSys/AddressingModes/AbsoluteLongAddressingMode.h"

AbsoluteLongAddressingMode::AbsoluteLongAddressingMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {

}

uint32_t AbsoluteLongAddressingMode::getAddress(uint8_t regAddr) {
    uint32_t address = bus->readLong(cpu->getPc());
    cpu->incrementPc(4);
    return address;
}

uint8_t AbsoluteLongAddressingMode::getModeId() {
    return MODE_ID;
}
