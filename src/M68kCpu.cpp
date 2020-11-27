//
// Created by paul.trampert on 11/8/2020.
//

#include <stdexcept>
#include <GenieSys/BriefExtensionWord.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>

M68kCpu::M68kCpu() = default;

M68kCpu::~M68kCpu() = default;

void M68kCpu::ConnectBus(Bus *bus) {
    this->bus = bus;
}

uint16_t M68kCpu::getCurrentOpWord() {
    return opWord;
}

uint32_t M68kCpu::getPc() {
    return pc;
}

void M68kCpu::incrementPc(int32_t amount) {
    pc += amount;
}

uint32_t M68kCpu::getDataRegister(uint8_t addr) {
    return dataRegisters[addr];
}

uint32_t M68kCpu::getAddressRegister(uint8_t addr) {
    return addressRegisters[addr];
}

void M68kCpu::setDataRegister(uint8_t addr, uint32_t value) {
    dataRegisters[addr] = value;
}

void M68kCpu::setAddressRegister(uint8_t addr, uint32_t value) {
    addressRegisters[addr] = value;
}

DATA_SIZE M68kCpu::getOperandSize() {
    return operandSize;
}
