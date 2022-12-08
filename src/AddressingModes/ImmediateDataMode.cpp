//
// Created by paul.trampert on 11/27/2020.
//

#include <memory>
#include <GenieSys/numberUtils.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include "GenieSys/AddressingModes/ImmediateDataMode.h"
#include "GenieSys/TrapException.h"


GenieSys::ImmediateDataMode::ImmediateDataMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : AddressingMode(cpu, bus) {
    cycles = 4;
    longCycles = 8;
}

uint32_t GenieSys::ImmediateDataMode::getAddress(uint8_t regAddr) {
    return cpu->getPc();
}

uint8_t GenieSys::ImmediateDataMode::getModeId() {
    return MODE_ID;
}

std::unique_ptr<GenieSys::AddressingResult> GenieSys::ImmediateDataMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    uint8_t incrSize = size;
    if (size == 1) {
        incrSize = 2;
        address++;
    }
    cpu->incrementPc(incrSize);
    return std::make_unique<GenieSys::AddressingResult>(cpu, bus, address, bus->read(address, size), size > 2 ? longCycles : cycles, this->getMoveCycleKey());
}

std::string GenieSys::ImmediateDataMode::disassemble(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    uint8_t incrSize = size;
    if (size == 1) {
        incrSize = 2;
        address++;
    }
    cpu->incrementPc(incrSize);
    return "$" + GenieSys::toHex(bus->read(address, size));
}

uint8_t GenieSys::ImmediateDataMode::getMoveCycleKey() {
    return 11;
}

std::unique_ptr<GenieSys::AddressingResult>
GenieSys::ImmediateDataMode::movemToReg(uint8_t regAddr, uint8_t size, uint16_t mask) {
    throw GenieSys::TrapException(TV_ILLEGAL_INSTR);
}
