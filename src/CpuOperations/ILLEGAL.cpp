//
// Created by pault on 11/3/2021.
//

#include <GenieSys/CpuOperations/ILLEGAL.h>
#include <GenieSys/M68kCpu.h>

GenieSys::ILLEGAL::ILLEGAL(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::ILLEGAL::getSpecificity() {
    return 0;
}

std::vector<uint16_t> GenieSys::ILLEGAL::getOpcodes() {
    return std::vector<uint16_t>{opWord};
}

uint8_t GenieSys::ILLEGAL::execute(uint16_t opWord) {
    cpu->trap(TV_ILLEGAL_INSTR);
    return 34;
}

std::string GenieSys::ILLEGAL::disassemble(uint16_t opWord) {
    return "ILLEGAL";
}
