//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterIndirectPostIncrementMode.h"

AddressRegisterIndirectPostIncrementMode::AddressRegisterIndirectPostIncrementMode(M68kCpu *cpu, Bus *bus)
        : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterIndirectPostIncrementMode::getAddress(uint8_t regAddr) {
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
    cpu->setAddressRegister(regAddr, address + size);
    return address;
}

uint8_t AddressRegisterIndirectPostIncrementMode::getModeId() {
    return 0b011u;
}
