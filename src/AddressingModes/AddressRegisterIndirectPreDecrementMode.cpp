//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h"

AddressRegisterIndirectPreDecrementMode::AddressRegisterIndirectPreDecrementMode(M68kCpu *cpu, Bus *bus)
        : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterIndirectPreDecrementMode::getAddress() {
    uint8_t regAddr = cpu->getCurrentOpWord() & EA_REG_MASK;
    uint32_t address = cpu->getAddressRegister(regAddr);
    uint8_t size = 1;
    switch (cpu->getOperandSize()) {
        case BYTE:
            size = regAddr == USP_ADDRESS ? 2 : 1;
            break;
        case WORD:
            size = 2;
            break;
        case LONG:
            size = 4;
            break;
    }
    address -= size;
    cpu->setAddressRegister(regAddr, address);
    return address;
}
