//
// Created by paul.trampert on 11/27/2020.
//

#include <memory>
#include <utility>
#include <GenieSys/M68kCpu.h>
#include "GenieSys/AddressingModes/DataRegisterDirectMode.h"
#include "GenieSys/numberUtils.h"
#include "GenieSys/TrapException.h"


GenieSys::DataRegisterDirectMode::DataRegisterDirectMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : AddressingMode(cpu, bus) {
    cycles = 0;
    longCycles = 0;
}

uint32_t GenieSys::DataRegisterDirectMode::getAddress(uint8_t regAddr) {
    return regAddr;
}

uint8_t GenieSys::DataRegisterDirectMode::getModeId() {
    return 0b000;
}

std::unique_ptr<GenieSys::AddressingResult> GenieSys::DataRegisterDirectMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t data = cpu->getDataRegister(regAddr);
    std::vector<uint8_t> result(size);
    for (int i = 0; i < size; i++) {
        uint32_t mask = 0xFF << (8 * ((size - 1) - i));
        result[i] = (data & mask) >> (8 * (size - 1 - i));
    }
    return std::make_unique<GenieSys::DataRegisterDirectResult>(cpu, bus, regAddr, result);
}

std::string GenieSys::DataRegisterDirectMode::disassemble(uint8_t regAddr, uint8_t size) {
    return "D" + std::to_string(regAddr);
}

std::unique_ptr<GenieSys::AddressingResult>
GenieSys::DataRegisterDirectMode::movemToReg(uint8_t regAddr, uint8_t size, uint16_t mask) {
    throw GenieSys::TrapException(TV_ILLEGAL_INSTR);
}

std::unique_ptr<GenieSys::AddressingResult>
GenieSys::DataRegisterDirectMode::movemToMem(uint8_t regAddr, uint8_t size, uint16_t mask) {
    throw GenieSys::TrapException(TV_ILLEGAL_INSTR);
}

GenieSys::DataRegisterDirectResult::DataRegisterDirectResult(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus, uint32_t address, std::vector<uint8_t> data)
        : AddressingResult(cpu, bus, address, std::move(data), 0, DataRegisterDirectMode::MODE_ID) {
}

void GenieSys::DataRegisterDirectResult::write(std::vector<uint8_t> data) {
    switch(data.size()) {
        case 1:
            cpu->setDataRegister(address, GenieSys::bytesToByte(data));
            break;
        case 2:
            cpu->setDataRegister(address, GenieSys::bytesToWord(data));
            break;
        case 4:
            cpu->setDataRegister(address, GenieSys::bytesToLong(data));
            break;
    }
}

void GenieSys::DataRegisterDirectResult::write(uint8_t data) {
    cpu->setDataRegister(address, data);
}

void GenieSys::DataRegisterDirectResult::write(uint16_t data) {
    cpu->setDataRegister(address, data);
}

void GenieSys::DataRegisterDirectResult::write(uint32_t data) {
    cpu->setDataRegister(address, data);
}

