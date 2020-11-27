//
// Created by paul.trampert on 11/27/2020.
//
#include <GenieSys/signExtend.h>
#include "GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h"

AddressRegisterIndirectDisplacementMode::AddressRegisterIndirectDisplacementMode(M68kCpu *cpu, Bus *bus)
        : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterIndirectDisplacementMode::getAddress() {
    auto displacement = signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return cpu->getAddressRegister(cpu->getCurrentOpWord() & EA_REG_MASK) + displacement;
}
