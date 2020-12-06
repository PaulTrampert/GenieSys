//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/AddressingModes/AddressingMode.h>

BitMask<uint16_t> AddressingMode::EA_MODE_MASK = BitMask<uint16_t>(5, 3);
BitMask<uint16_t> AddressingMode::EA_REG_MASK = BitMask<uint16_t>(2, 3);

AddressingMode::AddressingMode(M68kCpu *cpu, Bus *bus) {
    this->cpu = cpu;
    this->bus = bus;
}

void AddressingMode::setBus(Bus *b) {
    this->bus = b;
}

std::vector<uint8_t> AddressingMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    return bus->read(address, size);
}

