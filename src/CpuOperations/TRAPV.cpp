//
// Created by paul on 2/28/22.
//

#include <GenieSys/CpuOperations/TRAPV.h>
#include <GenieSys/M68kCpu.h>
#include <vector>

using namespace GenieSys;

GenieSys::TRAPV::TRAPV(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::TRAPV::getOpcodes() {
    return std::vector<uint16_t>{opWord};
}

uint8_t GenieSys::TRAPV::getSpecificity() {
    return 0;
}

uint8_t GenieSys::TRAPV::execute(uint16_t _) {
    if(cpu->getCcrFlags() & CCR_OVERFLOW) {
        return cpu->trap(TV_TRAPV);
    }
    return 4;
}

std::string TRAPV::disassemble(uint16_t opWord) {
    return "TRAPV";
}
