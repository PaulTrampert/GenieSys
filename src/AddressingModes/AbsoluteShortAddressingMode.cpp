//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/signExtend.h>
#include "GenieSys/AddressingModes/AbsoluteShortAddressingMode.h"

AbsoluteShortAddressingMode::AbsoluteShortAddressingMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {

}

uint32_t AbsoluteShortAddressingMode::getAddress(uint8_t regAddr) {
    uint32_t address = signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return address;
}

uint8_t AbsoluteShortAddressingMode::getModeId() {
    return 0b000u;
}
