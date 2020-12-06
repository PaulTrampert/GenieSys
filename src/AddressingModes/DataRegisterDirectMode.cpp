//
// Created by paul.trampert on 11/27/2020.
//

#include <memory>
#include <utility>
#include "GenieSys/AddressingModes/DataRegisterDirectMode.h"
#include "GenieSys/numberUtils.h"

DataRegisterDirectMode::DataRegisterDirectMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {

}

uint32_t DataRegisterDirectMode::getAddress(uint8_t regAddr) {
    return regAddr;
}

uint8_t DataRegisterDirectMode::getModeId() {
    return 0b000;
}

std::unique_ptr<AddressingResult> DataRegisterDirectMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t data = cpu->getDataRegister(regAddr);
    std::vector<uint8_t> result(size);
    for (int i = 0; i < size; i++) {
        uint32_t mask = 0xFF << (8 * ((size - 1) - i));
        result[i] = (data & mask) >> (8 * (size - 1 - i));
    }
    return std::make_unique<DataRegisterDirectResult>(cpu, bus, regAddr, result);
}

DataRegisterDirectResult::DataRegisterDirectResult(M68kCpu *cpu, Bus *bus, uint32_t address, std::vector<uint8_t> data)
        : AddressingResult(cpu, bus, address, std::move(data)) {
}

void DataRegisterDirectResult::write(std::vector<uint8_t> data) {
    switch(data.size()) {
        case 1:
            cpu->setDataRegister(address, data[0]);
            break;
        case 2:
            cpu->setDataRegister(address, bytesToWord(data));
            break;
        case 4:
            cpu->setDataRegister(address, bytesToLong(data));
            break;
    }
}

void DataRegisterDirectResult::write(uint8_t data) {
    cpu->setDataRegister(address, data);
}

void DataRegisterDirectResult::write(uint16_t data) {
    cpu->setDataRegister(address, data);
}

void DataRegisterDirectResult::write(uint32_t data) {
    cpu->setDataRegister(address, data);
}

