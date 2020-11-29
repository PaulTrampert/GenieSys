//
// Created by paul.trampert on 11/27/2020.
//
#include <GenieSys/signExtend.h>
#include "GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h"

AddressRegisterIndirectDisplacementMode::AddressRegisterIndirectDisplacementMode(M68kCpu *cpu, Bus *bus)
        : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterIndirectDisplacementMode::getAddress(uint8_t regAddr) {
    auto displacement = signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    return cpu->getAddressRegister(regAddr) + displacement;
}

uint8_t AddressRegisterIndirectDisplacementMode::getModeId() {
    return 0b101;
}
