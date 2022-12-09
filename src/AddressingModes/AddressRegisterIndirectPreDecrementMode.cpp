//
// Created by paul.trampert on 11/27/2020.
//

#include <memory>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include "GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h"
#include "GenieSys/TrapException.h"
#include "GenieSys/numberUtils.h"

GenieSys::AddressRegisterIndirectPreDecrementMode::AddressRegisterIndirectPreDecrementMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus)
        : GenieSys::AddressingMode(cpu, bus) {
    cycles = 6;
    longCycles = 10;
}

uint32_t GenieSys::AddressRegisterIndirectPreDecrementMode::getAddress(uint8_t regAddr) {
    return cpu->getAddressRegister(regAddr);
}

uint8_t GenieSys::AddressRegisterIndirectPreDecrementMode::getModeId() {
    return AddressRegisterIndirectPreDecrementMode::MODE_ID;
}

std::unique_ptr<GenieSys::AddressingResult> GenieSys::AddressRegisterIndirectPreDecrementMode::getData(uint8_t regAddr, uint8_t size) {
    uint32_t address = cpu->getAddressRegister(regAddr);
    uint8_t incrSize = size;
    if (size == 1 && regAddr == USP_ADDRESS) {
        incrSize = 2;
    }
    address -= incrSize;
    cpu->setAddressRegister(regAddr, address);
    return std::make_unique<GenieSys::AddressingResult>(cpu, bus, address, bus->read(address, size), size > 2 ? longCycles : cycles, this->getMoveCycleKey());
}

std::string GenieSys::AddressRegisterIndirectPreDecrementMode::disassemble(uint8_t regAddr, uint8_t size) {
    return "-(A" + std::to_string((int)regAddr) + ")";
}

std::unique_ptr<GenieSys::AddressingResult>
GenieSys::AddressRegisterIndirectPreDecrementMode::movemToReg(uint8_t regAddr, uint8_t size, uint16_t mask) {
    throw GenieSys::TrapException(TV_ILLEGAL_INSTR);
}

std::unique_ptr<GenieSys::AddressingResult>
GenieSys::AddressRegisterIndirectPreDecrementMode::movemToMem(uint8_t regAddr, uint8_t size, uint16_t mask) {
    uint32_t address = getAddress(regAddr);
    std::vector<uint8_t> data;
    uint8_t count = 0;
    int i = 15;
    while (mask > 0) {
        bool masked = mask % 2;
        if (masked) {
            address -= (++count * size);
            uint32_t nextElem;
            auto srcAddr = i > 7 ? i - 8 : i;
            if (i > 7) {
                nextElem = cpu->getAddressRegister(srcAddr);
            }
            else {
                nextElem = cpu->getDataRegister(srcAddr);
            }
            data.push_back(nextElem);
            if (size == 2) {
                bus->writeWord(address, nextElem & 0x0000FFFF);
            }
            else {
                bus->writeLong(address, nextElem);
            }
        }
        i--;
        mask = mask >> 1;
    }
    cpu->setAddressRegister(regAddr, address);
    return std::make_unique<AddressingResult>(cpu, bus, address, data, (size > 2 ? longCycles : cycles) * count, this->getMoveCycleKey());
}
