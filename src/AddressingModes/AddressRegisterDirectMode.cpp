//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/AddressRegisterDirectMode.h"

AddressRegisterDirectMode::AddressRegisterDirectMode(M68kCpu *cpu, Bus *bus)
    : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterDirectMode::getAddress(uint8_t regAddr) {
    return regAddr;
}

uint8_t AddressRegisterDirectMode::getModeId() {
    return MODE_ID;
}
