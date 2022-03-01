//
// Created by paul on 2/28/22.
//

#include <GenieSys/CpuOperations/RTR.h>
#include <GenieSys/M68kCpu.h>
#include <vector>

GenieSys::RTR::RTR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::RTR::getOpcodes() {
    return std::vector<uint16_t>{opWord};
}

uint8_t GenieSys::RTR::getSpecificity() {
    return 0;
}

uint8_t GenieSys::RTR::execute(uint16_t _) {
    cpu->setCcrFlags(cpu->stackPopWord() & 0x00FF);
    cpu->setPc(cpu->stackPopLong());
    return 20;
}

std::string GenieSys::RTR::disassemble(uint16_t _) {
    return "RTR";
}
