//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/DataRegisterDirectMode.h"

DataRegisterDirectMode::DataRegisterDirectMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {

}

uint32_t DataRegisterDirectMode::getAddress(uint8_t regAddr) {
    return regAddr;
}

uint8_t DataRegisterDirectMode::getModeId() {
    return 0b000;
}

std::vector<uint8_t> DataRegisterDirectMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t data = cpu->getDataRegister(regAddr);
    std::vector<uint8_t> result(size);
    for (int i = 0; i < size; i++) {
        uint32_t mask = 0xFF << (8 * ((size - 1) - i));
        result[i] = (data & mask) >> (8 * (size - 1 - i));
    }
    return result;
}

