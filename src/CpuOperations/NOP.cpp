//
// Created by paul.trampert on 12/16/2020.
//

#include <GenieSys/CpuOperations/NOP.h>



GenieSys::NOP::NOP(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::NOP::getOpcodes() {
    return std::vector<uint16_t>{0b0100111001110001};
}

uint8_t GenieSys::NOP::execute(uint16_t opWord) {
    return 1;
}

uint8_t GenieSys::NOP::getSpecificity() {
    return 0;
}

std::string GenieSys::NOP::disassemble(uint16_t opWord) {
    return "NOP";
}
