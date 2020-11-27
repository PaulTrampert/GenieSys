//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterIndirectMode.h"

AddressRegisterIndirectMode::AddressRegisterIndirectMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterIndirectMode::getAddress() {
    return cpu->getAddressRegister(cpu->getCurrentOpWord() & EA_REG_MASK);
}
