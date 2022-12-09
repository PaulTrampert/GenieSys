//
// Created by paul.trampert on 11/27/2020.
//
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/numberUtils.h>
#include <memory>
#include <utility>
#include <cassert>
#include "GenieSys/signExtend.h"


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

std::unique_ptr<GenieSys::AddressingResult> GenieSys::AddressingMode::movemToReg(uint8_t regAddr, uint8_t size, uint16_t mask) {
    uint32_t address = getAddress(regAddr);
    std::vector<uint8_t> data;
    uint8_t count = 0;
    int i = 0;
    while (mask > 0) {
        bool masked = mask % 2;
        if (masked) {
            uint32_t readAddr = address + (count++ * size);
            uint32_t nextElem;
            if (size == 2) {
                nextElem = signExtend<uint32_t>(bus->readWord(readAddr), 16);
            }
            else {
                nextElem = bus->readLong(readAddr);
            }
            data.push_back(nextElem);
            auto destAddr = i > 7 ? i - 8 : i;
            if (i > 7) {
                cpu->setAddressRegister(destAddr, nextElem);
            }
            else {
                cpu->setDataRegister(destAddr, nextElem);
            }
        }
        i++;
        mask = mask >> 1;
    }
    return std::make_unique<AddressingResult>(cpu, bus, address, data, (size > 2 ? longCycles : cycles) * count, this->getMoveCycleKey());
}

uint8_t GenieSys::AddressingMode::getMoveCycleKey() {
    return this->getModeId();
}

std::unique_ptr<GenieSys::AddressingResult> GenieSys::AddressingMode::movemToMem(uint8_t regAddr, uint8_t size, uint16_t mask) {
    return std::unique_ptr<AddressingResult>();
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
