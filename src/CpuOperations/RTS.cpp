//
// Created by paul on 2/28/22.
//

#include <GenieSys/CpuOperations/RTS.h>
#include <GenieSys/M68kCpu.h>
#include <vector>

GenieSys::RTS::RTS(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::RTS::getOpcodes() {
    return std::vector<uint16_t>{opWord};
}

uint8_t GenieSys::RTS::getSpecificity() {
    return 0;
}

uint8_t GenieSys::RTS::execute(uint16_t _) {
    cpu->setPc(cpu->stackPopLong());
    return 16;
}

std::string GenieSys::RTS::disassemble(uint16_t _) {
    return "RTS";
}
