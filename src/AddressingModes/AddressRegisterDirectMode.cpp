//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterDirectMode.h"



AddressRegisterDirectMode::AddressRegisterDirectMode(M68kCpu *cpu, Bus *bus)
    : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterDirectMode::getAddress(uint8_t regAddr) {
    return regAddr;
}

uint8_t AddressRegisterDirectMode::getModeId() {
    return MODE_ID;
}

std::vector<uint8_t> AddressRegisterDirectMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t data = cpu->getAddressRegister(regAddr);
    std::vector<uint8_t> result(size);
    for (int i = 0; i < size; i++) {
        uint32_t mask = 0xFF << (8 * ((size - 1) - i));
        result[i] = (data & mask) >> (8 * (size - 1 - i));
    }
    return result;
}
