//
// Created by paul on 6/20/24.
//
#include <GenieSys/CpuOperations/BRA.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/signExtend.h>
#include <GenieSys/M68kCpu.h>
#include <vector>
#include <sstream>

using namespace GenieSys;

GenieSys::BRA::BRA(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::BRA::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, &displacementMask);
}

uint8_t GenieSys::BRA::getSpecificity() {
    return displacementMask.getWidth();
}

uint8_t GenieSys::BRA::execute(uint16_t opWord) {
    auto displacement = signExtend<uint32_t>(displacementMask.apply(opWord), 8);
    if (displacement == 0) {
        displacement = bus->readWord(cpu->getPc());
        displacement = signExtend<uint32_t>(displacement, 16);
        cpu->incrementPc(2);
    }
    cpu->setPc(cpu->getPc() + displacement);

    return 10;
}

std::string BRA::disassemble(uint16_t opWord) {
    auto displacement = signExtend<uint32_t>(displacementMask.apply(opWord), 8);
    auto width = ".s";
    if (displacement == 0) {
        displacement = bus->readWord(cpu->getPc());
        displacement = signExtend<uint32_t>(displacement, 16);
        cpu->incrementPc(2);
        width = ".w";
    }
    std::stringstream str;
    str << "BRA" << width << " #" << std::to_string((int32_t)displacement);
    return str.str();
}
