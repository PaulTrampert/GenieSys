//
// Created by paul.trampert on 11/27/2020.
//
#include <GenieSys/signExtend.h>
#include "GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h"


AddressRegisterIndirectDisplacementMode::AddressRegisterIndirectDisplacementMode(GenieSys::M68kCpu *cpu, Bus *bus)
        : AddressingMode(cpu, bus) {
    cycles = 8;
    longCycles = 12;
}

uint32_t AddressRegisterIndirectDisplacementMode::getAddress(uint8_t regAddr) {
    auto displacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return cpu->getAddressRegister(regAddr) + displacement;
}

uint8_t AddressRegisterIndirectDisplacementMode::getModeId() {
    return MODE_ID;
}

std::string AddressRegisterIndirectDisplacementMode::disassemble(uint8_t regAddr, uint8_t size) {
    auto displacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return "(" + std::to_string(displacement) + ",A" + std::to_string(regAddr) + ")";
}
