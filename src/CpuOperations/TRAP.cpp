//
// Created by paul on 11/23/21.
//

#include <GenieSys/CpuOperations/TRAP.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <sstream>

GenieSys::TRAP::TRAP(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::TRAP::getSpecificity() {
    return vectorMask.getWidth();
}

std::vector<uint16_t> GenieSys::TRAP::getOpcodes() {
    return getPossibleOpcodes(baseOpCode, &vectorMask);
}

uint8_t GenieSys::TRAP::execute(uint16_t opWord) {
    uint8_t vector = vectorMask.apply(opWord);
    return cpu->trap(vector + 32);
}

std::string GenieSys::TRAP::disassemble(uint16_t opWord) {
    uint8_t vector = vectorMask.apply(opWord);
    std::stringstream stream;
    stream << "TRAP #" << (int)(vector + 32);
    return stream.str();
}
