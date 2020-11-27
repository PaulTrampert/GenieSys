//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/DataRegisterDirectMode.h"

DataRegisterDirectMode::DataRegisterDirectMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {

}

uint32_t DataRegisterDirectMode::getAddress() {
    return cpu->getCurrentOpWord() & EA_REG_MASK;
}

uint8_t DataRegisterDirectMode::getModeId() {
    return 0b000;
}

