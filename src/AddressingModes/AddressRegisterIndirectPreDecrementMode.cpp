//
// Created by paul.trampert on 11/27/2020.
//

#include <memory>
#include "GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h"



GenieSys::AddressRegisterIndirectPreDecrementMode::AddressRegisterIndirectPreDecrementMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus)
        : GenieSys::AddressingMode(cpu, bus) {
    cycles = 6;
    longCycles = 10;
}

uint32_t GenieSys::AddressRegisterIndirectPreDecrementMode::getAddress(uint8_t regAddr) {
    return cpu->getAddressRegister(regAddr);
}

uint8_t GenieSys::AddressRegisterIndirectPreDecrementMode::getModeId() {
    return AddressRegisterIndirectPreDecrementMode::MODE_ID;
}

std::unique_ptr<GenieSys::AddressingResult> GenieSys::AddressRegisterIndirectPreDecrementMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = cpu->getAddressRegister(regAddr);
    uint8_t incrSize = size;
    if (size == 1 && regAddr == USP_ADDRESS) {
        incrSize = 2;
    }
    address -= incrSize;
    cpu->setAddressRegister(regAddr, address);
    return std::make_unique<GenieSys::AddressingResult>(cpu, bus, address, bus->read(address, size), size > 2 ? longCycles : cycles, this->getMoveCycleKey());
}

std::string GenieSys::AddressRegisterIndirectPreDecrementMode::disassemble(uint8_t regAddr, uint8_t size) {
    return "-(A" + std::to_string((int)regAddr) + ")";
}
