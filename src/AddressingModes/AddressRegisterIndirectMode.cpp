//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterIndirectMode.h"



GenieSys::AddressRegisterIndirectMode::AddressRegisterIndirectMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : AddressingMode(cpu, bus) {
    cycles = 4;
    longCycles = 8;
}

uint32_t GenieSys::AddressRegisterIndirectMode::getAddress(uint8_t regAddr) {
    return cpu->getAddressRegister(regAddr);
}

uint8_t GenieSys::AddressRegisterIndirectMode::getModeId() {
    return MODE_ID;
}

std::string GenieSys::AddressRegisterIndirectMode::disassemble(uint8_t regAddr, uint8_t size) {
    return "(A" + std::to_string(regAddr) + ")";
}
