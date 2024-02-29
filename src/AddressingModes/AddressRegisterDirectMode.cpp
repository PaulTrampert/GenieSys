//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterDirectMode.h"
#include "GenieSys/M68kCpu.h"
#include "GenieSys/TrapException.h"
#include <utility>
#include <GenieSys/numberUtils.h>


GenieSys::AddressRegisterDirectMode::AddressRegisterDirectMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus)
    : AddressingMode(cpu, bus) {
    cycles = 0;
    longCycles = 0;
}

uint32_t GenieSys::AddressRegisterDirectMode::getAddress(uint8_t regAddr) {
    return regAddr;
}

uint8_t GenieSys::AddressRegisterDirectMode::getModeId() {
    return MODE_ID;
}

std::unique_ptr<GenieSys::AddressingResult> GenieSys::AddressRegisterDirectMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t data = cpu->getAddressRegister(regAddr);
    std::vector<uint8_t> result(size);
    for (int i = 0; i < size; i++) {
        uint32_t mask = 0xFF << (8 * ((size - 1) - i));
        result[i] = (data & mask) >> (8 * (size - 1 - i));
    }
    return std::make_unique<GenieSys::AddressRegisterDirectResult>(cpu, bus, regAddr, result);
}

std::string GenieSys::AddressRegisterDirectMode::disassemble(uint8_t regAddr, uint8_t size) {
    return "A" + std::to_string(regAddr);
}

std::unique_ptr<GenieSys::AddressingResult>
GenieSys::AddressRegisterDirectMode::movemToReg(uint8_t regAddr, uint8_t size, uint16_t mask) {
    throw GenieSys::TrapException(TV_ILLEGAL_INSTR);
}

std::unique_ptr<GenieSys::AddressingResult>
GenieSys::AddressRegisterDirectMode::movemToMem(uint8_t regAddr, uint8_t size, uint16_t mask) {
    throw GenieSys::TrapException(TV_ILLEGAL_INSTR);
}

GenieSys::AddressRegisterDirectResult::AddressRegisterDirectResult(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus, uint32_t address, std::vector<uint8_t> data)
    : AddressingResult(cpu, bus, address, std::move(data), 0, AddressRegisterDirectMode::MODE_ID) {

}

void GenieSys::AddressRegisterDirectResult::write(std::vector<uint8_t> data) {
    switch(data.size()) {
        case 1:
            cpu->setAddressRegister(address, GenieSys::bytesToByte(data));
            break;
        case 2:
            cpu->setAddressRegister(address, GenieSys::bytesToWord(data));
            break;
        case 4:
            cpu->setAddressRegister(address, GenieSys::bytesToLong(data));
            break;
    }
}

void GenieSys::AddressRegisterDirectResult::write(uint8_t data) {
    cpu->setAddressRegister(address, data);
}

void GenieSys::AddressRegisterDirectResult::write(uint16_t data) {
    cpu->setAddressRegister(address, data);
}

void GenieSys::AddressRegisterDirectResult::write(uint32_t data) {
    cpu->setAddressRegister(address, data);
}
