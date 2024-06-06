//
// Created by paul on 6/5/24.
//
#include <GenieSys/CpuOperations/BSR.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/signExtend.h>
#include <GenieSys/numberUtils.h>
#include <sstream>

GenieSys::BSR::BSR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::BSR::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, std::vector<BitMask<uint16_t>*>{
        &displacementMask
    });
}

uint8_t GenieSys::BSR::getSpecificity() {
    return displacementMask.getWidth();
}

uint8_t GenieSys::BSR::execute(uint16_t opWord) {
    uint32_t displacement = displacementMask.apply(opWord);
    uint8_t bits = 8;
    if (displacement == 0) {
        displacement = bus->readWord(cpu->getPc());
        cpu->incrementPc(2);
        bits = 16;
    }
    cpu->stackPushLong(cpu->getPc());
    displacement = signExtend<uint32_t>(displacement, bits);
    cpu->incrementPc((int32_t)displacement);
    return 18;
}

std::string GenieSys::BSR::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint16_t displacement = displacementMask.apply(opWord);
    uint8_t bits = 8;
    if (displacement == 0) {
        displacement = bus->readWord(cpu->getPc());
        cpu->incrementPc(2);
        bits = 16;
    }
    displacement = signExtend<uint16_t>(displacement, bits);

    stream << "BSR $" << toHex(getBytes(displacement));
    return stream.str();
}
