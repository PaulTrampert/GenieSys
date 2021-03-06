//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/numberUtils.h>
#include <memory>
#include <utility>

BitMask<uint16_t> AddressingMode::EA_MODE_MASK = BitMask<uint16_t>(5, 3);
BitMask<uint16_t> AddressingMode::EA_REG_MASK = BitMask<uint16_t>(2, 3);

AddressingMode::AddressingMode(M68kCpu *cpu, Bus *bus) {
    this->cpu = cpu;
    this->bus = bus;
}

void AddressingMode::setBus(Bus *b) {
    this->bus = b;
}

std::unique_ptr<AddressingResult> AddressingMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    auto data = bus->read(address, size);
    return std::make_unique<AddressingResult>( cpu, bus, address, data, size > 2 ? longCycles : cycles );
}

AddressingResult::AddressingResult(M68kCpu *cpu, Bus *bus, uint32_t address, std::vector<uint8_t> data,
                                   uint8_t cycles) {
    this->cpu = cpu;
    this->bus = bus;
    this->address = address;
    this->data = std::move(data);
    this->cycles = cycles;
}

std::vector<uint8_t> AddressingResult::getData() {
    return data;
}

uint8_t AddressingResult::getDataAsByte() {
    return bytesToByte(data);
}

uint16_t AddressingResult::getDataAsWord() {
    return bytesToWord(data);
}

uint32_t AddressingResult::getDataAsLong() {
    return bytesToLong(data);
}

void AddressingResult::write(std::vector<uint8_t> data) {
    bus->write(address, std::move(data));
}

void AddressingResult::write(uint8_t data) {
    bus->writeByte(address, data);
}

void AddressingResult::write(uint16_t data) {
    bus->writeWord(address, data);
}

void AddressingResult::write(uint32_t data) {
    bus->writeLong(address, data);
}

uint8_t AddressingResult::getCycles() {
    return cycles;
}
