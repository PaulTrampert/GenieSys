//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h"

AddressRegisterIndirectPreDecrementMode::AddressRegisterIndirectPreDecrementMode(M68kCpu *cpu, Bus *bus)
        : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterIndirectPreDecrementMode::getAddress(uint8_t regAddr) {
    return cpu->getAddressRegister(regAddr);
}

uint8_t AddressRegisterIndirectPreDecrementMode::getModeId() {
    return AddressRegisterIndirectPreDecrementMode::MODE_ID;
}

std::vector<uint8_t> AddressRegisterIndirectPreDecrementMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = cpu->getAddressRegister(regAddr);
    uint8_t incrSize = size;
    if (size == 1 && regAddr == USP_ADDRESS) {
        incrSize = 2;
    }
    address -= incrSize;
    cpu->setAddressRegister(regAddr, address);
    return bus->read(address, size);
}
