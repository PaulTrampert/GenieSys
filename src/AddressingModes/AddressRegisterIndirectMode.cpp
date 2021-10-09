//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterIndirectMode.h"


AddressRegisterIndirectMode::AddressRegisterIndirectMode(GenieSys::M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {
    cycles = 4;
    longCycles = 8;
}

uint32_t AddressRegisterIndirectMode::getAddress(uint8_t regAddr) {
    return cpu->getAddressRegister(regAddr);
}

uint8_t AddressRegisterIndirectMode::getModeId() {
    return MODE_ID;
}

std::string AddressRegisterIndirectMode::disassemble(uint8_t regAddr, uint8_t size) {
    return "(A" + std::to_string(regAddr) + ")";
}
