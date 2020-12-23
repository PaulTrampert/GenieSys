//
// Created by paul.trampert on 11/8/2020.
//

#include <stdexcept>
#include <memory>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPostIncrementMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectWithIndexMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/CpuOperations/CpuOperation.h>
#include <GenieSys/CpuOperations/Nop.h>

M68kCpu::M68kCpu() {
    for (auto & mode : addressingModes) {
        mode = nullptr;
    }
    addressingModes[DataRegisterDirectMode::MODE_ID] = std::unique_ptr<AddressingMode>(new DataRegisterDirectMode(this, bus));
    addressingModes[AddressRegisterDirectMode::MODE_ID] = std::unique_ptr<AddressingMode>(new AddressRegisterDirectMode(this, bus));
    addressingModes[AddressRegisterIndirectMode::MODE_ID] = std::unique_ptr<AddressingMode>(new AddressRegisterIndirectMode(this, bus));
    addressingModes[AddressRegisterIndirectDisplacementMode::MODE_ID] = std::unique_ptr<AddressingMode>(new AddressRegisterIndirectDisplacementMode(this, bus));
    addressingModes[AddressRegisterIndirectPostIncrementMode::MODE_ID] = std::unique_ptr<AddressingMode>(new AddressRegisterIndirectPostIncrementMode(this, bus));
    addressingModes[AddressRegisterIndirectPreDecrementMode::MODE_ID] = std::unique_ptr<AddressingMode>(new AddressRegisterIndirectPreDecrementMode(this, bus));
    addressingModes[AddressRegisterIndirectWithIndexMode::MODE_ID] = std::unique_ptr<AddressingMode>(new AddressRegisterIndirectWithIndexMode(this, bus));
    addressingModes[ProgramCounterAddressingMode::MODE_ID] = std::unique_ptr<AddressingMode>(new ProgramCounterAddressingMode(this, bus));

    nop = std::shared_ptr<CpuOperation>(new Nop(this, bus));
    std::fill(opTable.begin(), opTable.end(), nop);
    for (auto & op : getOperations(this, bus)) {
        for (auto & opcode : op->getOpcodes()) {
            opTable[opcode] = op;
        }
    }
}

M68kCpu::~M68kCpu() = default;

void M68kCpu::ConnectBus(Bus *bus) {
    this->bus = bus;
    for (auto & mode : addressingModes) {
        if (mode != nullptr) {
            mode->setBus(bus);
        }
    }
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

void M68kCpu::setDataRegister(uint8_t addr, uint8_t value) {
    dataRegisters[addr] = (dataRegisters[addr] & 0xFFFFFF00) | value;
}

void M68kCpu::setDataRegister(uint8_t addr, uint16_t value) {
    dataRegisters[addr] = (dataRegisters[addr] & 0xFFFF0000) | value;
}

void M68kCpu::setDataRegister(uint8_t addr, uint32_t value) {
    dataRegisters[addr] = value;
}

void M68kCpu::setAddressRegister(uint8_t addr, uint32_t value) {
    addressRegisters[addr] = value;
}

void M68kCpu::setPc(uint32_t value) {
    pc = value;
}

void M68kCpu::tick() {
    if (clock == 0) {
        opWord = bus->readWord(pc);
        pc += 2;
        clock = opTable[opWord]->execute(opWord);
    }
    clock--;
}

AddressingMode *M68kCpu::getAddressingMode(int modeId) {
    return addressingModes[modeId].get();
}

void M68kCpu::setCcrFlags(uint8_t ccr) {
    this->ccr = ccr;
}

uint8_t M68kCpu::getCcrFlags() {
    return ccr;
}





