//
// Created by paul.trampert on 11/27/2020.
//

#include <memory>
#include "GenieSys/AddressingModes/AddressRegisterIndirectPostIncrementMode.h"



GenieSys::AddressRegisterIndirectPostIncrementMode::AddressRegisterIndirectPostIncrementMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus)
        : GenieSys::AddressingMode(cpu, bus) {
    cycles = 4;
    longCycles = 8;
}

uint32_t GenieSys::AddressRegisterIndirectPostIncrementMode::getAddress(uint8_t regAddr) {
    return cpu->getAddressRegister(regAddr);
}

uint8_t GenieSys::AddressRegisterIndirectPostIncrementMode::getModeId() {
    return MODE_ID;
}

std::unique_ptr<GenieSys::AddressingResult> GenieSys::AddressRegisterIndirectPostIncrementMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    uint8_t incrSize = size;
    if (size == 1 && regAddr == USP_ADDRESS) {
        incrSize = 2;
    }
    cpu->setAddressRegister(regAddr, address + incrSize);
    return std::make_unique<GenieSys::AddressingResult>(cpu, bus, address, bus->read(address, size), size > 2 ? longCycles : cycles, this->getMoveCycleKey());
}

std::string GenieSys::AddressRegisterIndirectPostIncrementMode::disassemble(uint8_t regAddr, uint8_t size) {
    return "(A" + std::to_string(regAddr) + ")+";
}
