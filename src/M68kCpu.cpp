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
#include <GenieSys/CpuOperations/NOP.h>

#define SP addressRegisters[7]

GenieSys::M68kCpu::M68kCpu() {
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

    nop = std::shared_ptr<CpuOperation>(new NOP(this, bus));
    std::fill(opTable.begin(), opTable.end(), nop);
    for (auto &op : getOperations(this, bus)) {
        for (auto & opcode : op->getOpcodes()) {
            opTable[opcode] = op;
        }
    }
}

GenieSys::M68kCpu::~M68kCpu() = default;

void GenieSys::M68kCpu::ConnectBus(Bus *bus) {
    this->bus = bus;
    for (auto &mode : addressingModes) {
        if (mode != nullptr) {
            mode->setBus(bus);
        }
    }
    opTable.clear();
    nop = std::shared_ptr<CpuOperation>(new NOP(this, bus));
    std::fill(opTable.begin(), opTable.end(), nop);
    for (auto &op : getOperations(this, bus)) {
        for (auto & opcode : op->getOpcodes()) {
            opTable[opcode] = op;
        }
    }
}

uint16_t GenieSys::M68kCpu::getCurrentOpWord() {
    return opWord;
}

uint32_t GenieSys::M68kCpu::getPc() {
    return pc;
}

void GenieSys::M68kCpu::incrementPc(int32_t amount) {
    pc += amount;
}

uint32_t GenieSys::M68kCpu::getDataRegister(uint8_t addr) {
    return dataRegisters[addr];
}

uint32_t GenieSys::M68kCpu::getAddressRegister(uint8_t addr) {
    if (addr == 7 && isSupervisor()) {
        return ssp;
    }
    return addressRegisters[addr];
}

void GenieSys::M68kCpu::setDataRegister(uint8_t addr, uint8_t value) {
    dataRegisters[addr] = (dataRegisters[addr] & 0xFFFFFF00) | value;
}

void GenieSys::M68kCpu::setDataRegister(uint8_t addr, uint16_t value) {
    dataRegisters[addr] = (dataRegisters[addr] & 0xFFFF0000) | value;
}

void GenieSys::M68kCpu::setDataRegister(uint8_t addr, uint32_t value) {
    dataRegisters[addr] = value;
}

void GenieSys::M68kCpu::setAddressRegister(uint8_t addr, uint32_t value) {
    if (addr == 7 && isSupervisor()) {
        ssp = value;
    }
    else {
        addressRegisters[addr] = value;
    }
}

void GenieSys::M68kCpu::setPc(uint32_t value) {
    pc = value;
}

void GenieSys::M68kCpu::tick() {
    if (clock == 0) {
        opWord = bus->readWord(pc);
        pc += 2;
#ifdef DISASSEMBLE
        uint32_t oldPc = pc;
        std::string instr = opTable[opWord]->disassemble(opWord);
        printf("%#08x    %s", oldPc, instr.c_str());
        pc = oldPc;
#endif
        clock = opTable[opWord]->execute(opWord);
    }
    clock--;
}

GenieSys::AddressingMode *GenieSys::M68kCpu::getAddressingMode(int modeId) {
    return addressingModes[modeId].get();
}

void GenieSys::M68kCpu::setCcrFlags(uint8_t ccr) {
    this->SRandCCR = (SRandCCR & 0xFF00) | ccr;
}

uint8_t GenieSys::M68kCpu::getCcrFlags() {
    return SRandCCR & 0x00FF;
}

uint16_t GenieSys::M68kCpu::getSR() {
    return SRandCCR;
}

void GenieSys::M68kCpu::setSR(uint16_t sr) {
    SRandCCR = sr;
}

bool GenieSys::M68kCpu::isSupervisor() {
    return supervisorState.apply(SRandCCR) == 1;
}

uint8_t GenieSys::M68kCpu::trap(uint8_t vector) {
    SRandCCR = supervisorState.compose(SRandCCR, 1);
    stackPush(pc);
    stackPush(SRandCCR);
    pc = bus->readLong(4 * vector);
    return 34;
}

uint8_t GenieSys::M68kCpu::getUspRegister() {
    return 7;
}

void GenieSys::M68kCpu::stackPush(uint16_t value) {
    uint32_t spAddr = this->getAddressRegister(getUspRegister());
    spAddr -= 2;
    this->setAddressRegister(getUspRegister(), spAddr);
    bus->writeWord(spAddr, value);
}

void GenieSys::M68kCpu::stackPush(uint32_t value) {
    uint32_t spAddr = this->getAddressRegister(getUspRegister());
    spAddr -= 4;
    this->setAddressRegister(getUspRegister(), spAddr);
    bus->writeLong(spAddr, value);
}

uint16_t GenieSys::M68kCpu::stackPopWord() {
    uint32_t spAddr = this->getAddressRegister(getUspRegister());
    auto result = bus->readWord(spAddr);
    spAddr += 2;
    this->setAddressRegister(getUspRegister(), spAddr);
    return result;
}

uint32_t GenieSys::M68kCpu::stackPopLong() {
    uint32_t spAddr = this->getAddressRegister(getUspRegister());
    auto result = bus->readLong(spAddr);
    spAddr += 4;
    this->setAddressRegister(getUspRegister(), spAddr);
    return result;
}

uint32_t GenieSys::M68kCpu::getStackPointer() {
    return getAddressRegister(getUspRegister());
}

void GenieSys::M68kCpu::setStackPointer(uint32_t p) {
    setAddressRegister(getUspRegister(), p);
}

void GenieSys::M68kCpu::setSupervisor(bool isSupervisor) {
    SRandCCR = supervisorState.compose(this->SRandCCR, isSupervisor ? 1 : 0);
}

uint32_t GenieSys::M68kCpu::getUserStackPointer() {
    return addressRegisters[getUspRegister()];
}

void GenieSys::M68kCpu::setUserStackPointer(uint32_t addr) {
    addressRegisters[getUspRegister()] = addr;
}

void GenieSys::M68kCpu::reset() {
    pc = bus->readLong(TV_INIT_PC * 4);
}
