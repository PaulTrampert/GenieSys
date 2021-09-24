//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterDirectMode.h"

#include <utility>
#include <GenieSys/numberUtils.h>


AddressRegisterDirectMode::AddressRegisterDirectMode(M68kCpu *cpu, Bus *bus)
    : AddressingMode(cpu, bus) {
    cycles = 0;
    longCycles = 0;
}

uint32_t AddressRegisterDirectMode::getAddress(uint8_t regAddr) {
    return regAddr;
}

uint8_t AddressRegisterDirectMode::getModeId() {
    return MODE_ID;
}

std::unique_ptr<AddressingResult> AddressRegisterDirectMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t data = cpu->getAddressRegister(regAddr);
    std::vector<uint8_t> result(size);
    for (int i = 0; i < size; i++) {
        uint32_t mask = 0xFF << (8 * ((size - 1) - i));
        result[i] = (data & mask) >> (8 * (size - 1 - i));
    }
    return std::make_unique<AddressRegisterDirectResult>(cpu, bus, regAddr, result);
}

std::string AddressRegisterDirectMode::disassemble(uint8_t regAddr, uint8_t size) {
    return "A" + std::to_string(regAddr);
}

AddressRegisterDirectResult::AddressRegisterDirectResult(M68kCpu *cpu, Bus *bus, uint32_t address, std::vector<uint8_t> data)
    : AddressingResult(cpu, bus, address, std::move(data), 0, AddressRegisterDirectMode::MODE_ID) {

}

void AddressRegisterDirectResult::write(std::vector<uint8_t> data) {
    switch(data.size()) {
        case 1:
            cpu->setAddressRegister(address, bytesToByte(data));
            break;
        case 2:
            cpu->setAddressRegister(address, bytesToWord(data));
            break;
        case 4:
            cpu->setAddressRegister(address, bytesToLong(data));
            break;
    }
}

void AddressRegisterDirectResult::write(uint8_t data) {
    cpu->setAddressRegister(address, data);
}

void AddressRegisterDirectResult::write(uint16_t data) {
    cpu->setAddressRegister(address, data);
}

void AddressRegisterDirectResult::write(uint32_t data) {
    cpu->setAddressRegister(address, data);
}
