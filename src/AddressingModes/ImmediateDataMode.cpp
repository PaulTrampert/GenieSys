//
// Created by paul.trampert on 11/27/2020.
//

#include "GenieSys/AddressingModes/ImmediateDataMode.h"

ImmediateDataMode::ImmediateDataMode(M68kCpu *cpu, Bus *bus) : AddressingMode(cpu, bus) {

}

uint32_t ImmediateDataMode::getAddress() {
    return cpu->getPc();
}
