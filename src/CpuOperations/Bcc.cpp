//
// Created by paul on 5/28/24.
//
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <GenieSys/CpuOperations/Bcc.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include "GenieSys/signExtend.h"
#include "GenieSys/ConditionCodes.h"

GenieSys::Bcc::Bcc(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::Bcc::getOpcodes() {
    return getPossibleOpcodes(baseOpcode, std::vector<GenieSys::BitMask<uint16_t>*> {
        &conditionMask,
        &displacementMask
    });
}

uint8_t GenieSys::Bcc::getSpecificity() {
    return conditionMask.getWidth() + displacementMask.getWidth();
}

uint8_t GenieSys::Bcc::execute(uint16_t opWord) {
    uint8_t condition = conditionMask.apply(opWord);
    auto displacement = signExtend<uint32_t>((uint8_t)displacementMask.apply(opWord), 8);
    uint8_t cycles = 8;
    uint32_t pc = cpu->getPc();
    if (displacement == 0) {
        displacement = signExtend<uint32_t>(bus->readWord(pc), 16);
        cpu->incrementPc(2);
        cycles += 4;
        pc = cpu->getPc();
    }
    if (condition == CC_T || condition == CC_F) {
        return cpu->trap(TV_ILLEGAL_INSTR);
    }
    if (cpu->testConditionCode(condition)) {
        cpu->setPc(pc + displacement);
        return 10;
    }
    return cycles;
}

std::string GenieSys::Bcc::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t condition = conditionMask.apply(opWord);
    auto displacement = signExtend<int16_t>((int16_t) displacementMask.apply(opWord), 8);
    if (displacement == 0) {
        displacement = (int16_t) bus->readWord(cpu->getPc());
        cpu->incrementPc(2);
    }
    stream << "B" << getConditionCodeName(condition) << " " << std::to_string(displacement);
    return stream.str();
}
