//
// Created by paul on 2/28/22.
//
#include <GenieSys/CpuOperations/RTE.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/enums.h>
#include <vector>

GenieSys::RTE::RTE(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::RTE::getOpcodes() {
    return std::vector<uint16_t>{opWord};
}

uint8_t GenieSys::RTE::getSpecificity() {
    return 0;
}

uint8_t GenieSys::RTE::execute(uint16_t opWord) {
    if (!cpu->isSupervisor())
        return cpu->trap(GenieSys::TV_PRIVILEGE);
    cpu->setSR(cpu->stackPopWord());
    cpu->setPc(cpu->stackPopLong());
    return 20;
}

std::string GenieSys::RTE::disassemble(uint16_t opWord) {
    return "RTE";
}
