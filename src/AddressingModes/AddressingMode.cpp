//
// Created by paul.trampert on 11/27/2020.
//
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/numberUtils.h>
#include <memory>
#include <utility>



GenieSys::BitMask<uint16_t> GenieSys::AddressingMode::EA_MODE_MASK = GenieSys::BitMask<uint16_t>(5, 3);
GenieSys::BitMask<uint16_t> GenieSys::AddressingMode::EA_REG_MASK = GenieSys::BitMask<uint16_t>(2, 3);

GenieSys::AddressingMode::AddressingMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) {
    this->cpu = cpu;
    this->bus = bus;
}

void GenieSys::AddressingMode::setBus(GenieSys::Bus *b) {
    this->bus = b;
}

std::unique_ptr<GenieSys::AddressingResult> GenieSys::AddressingMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = getAddress(regAddr);
    auto data = bus->read(address, size);
    return std::make_unique<AddressingResult>( cpu, bus, address, data, size > 2 ? longCycles : cycles, this->getMoveCycleKey());
}

uint8_t GenieSys::AddressingMode::getMoveCycleKey() {
    return this->getModeId();
}

GenieSys::AddressingResult::AddressingResult(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus, uint32_t address, std::vector<uint8_t> data,
                                             uint8_t cycles, uint8_t moveCycleKey) {
    this->cpu = cpu;
    this->bus = bus;
    this->address = address;
    this->data = std::move(data);
    this->cycles = cycles;
    this->moveCycleKey = moveCycleKey;
}

std::vector<uint8_t> GenieSys::AddressingResult::getData() {
    return data;
}

uint8_t GenieSys::AddressingResult::getDataAsByte() {
    return GenieSys::bytesToByte(data);
}

uint16_t GenieSys::AddressingResult::getDataAsWord() {
    return GenieSys::bytesToWord(data);
}

uint32_t GenieSys::AddressingResult::getDataAsLong() {
    return GenieSys::bytesToLong(data);
}

void GenieSys::AddressingResult::write(std::vector<uint8_t> data) {
    bus->write(address, std::move(data));
}

void GenieSys::AddressingResult::write(uint8_t data) {
    bus->writeByte(address, data);
}

void GenieSys::AddressingResult::write(uint16_t data) {
    bus->writeWord(address, data);
}

void GenieSys::AddressingResult::write(uint32_t data) {
    bus->writeLong(address, data);
}

uint8_t GenieSys::AddressingResult::getCycles() {
    return cycles;
}

uint8_t GenieSys::AddressingResult::getMoveCycleKey() {
    return moveCycleKey;
}

uint32_t GenieSys::AddressingResult::getAddress() {
    return address;
}
