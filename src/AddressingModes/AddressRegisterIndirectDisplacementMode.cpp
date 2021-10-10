//
// Created by paul.trampert on 11/27/2020.
//
#include <GenieSys/signExtend.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include "GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h"



GenieSys::AddressRegisterIndirectDisplacementMode::AddressRegisterIndirectDisplacementMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus)
        : GenieSys::AddressingMode(cpu, bus) {
    cycles = 8;
    longCycles = 12;
}

uint32_t GenieSys::AddressRegisterIndirectDisplacementMode::getAddress(uint8_t regAddr) {
    auto displacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return cpu->getAddressRegister(regAddr) + displacement;
}

uint8_t GenieSys::AddressRegisterIndirectDisplacementMode::getModeId() {
    return MODE_ID;
}

std::string GenieSys::AddressRegisterIndirectDisplacementMode::disassemble(uint8_t regAddr, uint8_t size) {
    auto displacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return "(" + std::to_string(displacement) + ",A" + std::to_string(regAddr) + ")";
}
