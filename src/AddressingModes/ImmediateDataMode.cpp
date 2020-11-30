//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/ImmediateDataMode.h"

ImmediateDataMode::ImmediateDataMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {

}

uint32_t ImmediateDataMode::getAddress(uint8_t regAddr) {
    return cpu->getPc();
}

uint8_t ImmediateDataMode::getModeId() {
    return 0b100u;
}

std::vector<uint8_t> ImmediateDataMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    uint8_t incrSize = size;
    if (size == 1) {
        incrSize = 2;
        address++;
    }
    cpu->incrementPc(incrSize);
    return bus->read(address, size);
}
