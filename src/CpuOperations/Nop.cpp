//
// Created by paul.trampert on 12/16/2020.
//

#include <GenieSys/CpuOperations/Nop.h>

Nop::Nop(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> Nop::getOpcodes() {
    return std::vector<uint16_t>{0b0100111001110001};
}

uint8_t Nop::execute(uint16_t opWord) {
    return 1;
}

uint8_t Nop::getSpecificity() {
    return 0;
}

std::string Nop::disassemble(uint16_t opWord) {
    return "NOP";
}
