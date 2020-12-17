//
// Created by paul.trampert on 12/16/2020.
//

#include <GenieSys/CpuOperations/Nop.h>

Nop::Nop(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> Nop::getOpcodes() {
    return std::vector<uint16_t>{0b0100111001110001};
}

void Nop::execute(uint16_t opWord) {
}

uint8_t Nop::getSpecificity() {
    return 0;
}
