//
// Created by pault on 12/28/2021.
//

#include <GenieSys/CpuOperations/RESET.h>
#include <GenieSys/Bus.h>
#include <vector>

GenieSys::RESET::RESET(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::RESET::getOpcodes() {
    return std::vector<uint16_t>{opWord};
}

uint8_t GenieSys::RESET::getSpecificity() {
    return 0;
}

uint8_t GenieSys::RESET::execute(uint16_t opWord) {
    bus->reset();
    return 124;
}

std::string GenieSys::RESET::disassemble(uint16_t opWord) {
    return "RESET";
}
