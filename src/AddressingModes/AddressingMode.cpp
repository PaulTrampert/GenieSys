//
// Created by paul.trampert on 11/27/2020.
//

#include <GenieSys/AddressingModes/AddressingMode.h>

AddressingMode::AddressingMode(M68kCpu *cpu, Bus *bus) {
    this->cpu = cpu;
    this->bus = bus;
}

