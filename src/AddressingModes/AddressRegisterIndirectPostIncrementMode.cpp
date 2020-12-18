//
// Created by paul.trampert on 11/27/2020.
//

#include <memory>
#include "GenieSys/AddressingModes/AddressRegisterIndirectPostIncrementMode.h"

AddressRegisterIndirectPostIncrementMode::AddressRegisterIndirectPostIncrementMode(M68kCpu *cpu, Bus *bus)
        : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterIndirectPostIncrementMode::getAddress(uint8_t regAddr) {
    return cpu->getAddressRegister(regAddr);
}

uint8_t AddressRegisterIndirectPostIncrementMode::getModeId() {
    return MODE_ID;
}

std::unique_ptr<AddressingResult> AddressRegisterIndirectPostIncrementMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    uint8_t incrSize = size;
    if (size == 1 && regAddr == USP_ADDRESS) {
        incrSize = 2;
    }
    cpu->setAddressRegister(regAddr, address + incrSize);
    return std::make_unique<AddressingResult>(cpu, bus, address, bus->read(address, size));
}

std::string AddressRegisterIndirectPostIncrementMode::disassemble(uint8_t regAddr) {
    return std::string();
}
