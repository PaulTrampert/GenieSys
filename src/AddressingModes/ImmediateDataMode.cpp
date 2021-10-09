//
// Created by paul.trampert on 11/27/2020.
//

#include <memory>
#include <GenieSys/numberUtils.h>
#include "GenieSys/AddressingModes/ImmediateDataMode.h"

ImmediateDataMode::ImmediateDataMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {
    cycles = 4;
    longCycles = 8;
}

uint32_t ImmediateDataMode::getAddress(uint8_t regAddr) {
    return cpu->getPc();
}

uint8_t ImmediateDataMode::getModeId() {
    return MODE_ID;
}

std::unique_ptr<AddressingResult> ImmediateDataMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    uint8_t incrSize = size;
    if (size == 1) {
        incrSize = 2;
        address++;
    }
    cpu->incrementPc(incrSize);
    return std::make_unique<AddressingResult>(cpu, bus, address, bus->read(address, size), size > 2 ? longCycles : cycles, this->getMoveCycleKey());
}

std::string ImmediateDataMode::disassemble(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    uint8_t incrSize = size;
    if (size == 1) {
        incrSize = 2;
        address++;
    }
    cpu->incrementPc(incrSize);
    return "$" + GenieSys::toHex(bus->read(address, size));
}

uint8_t ImmediateDataMode::getMoveCycleKey() {
    return 11;
}
