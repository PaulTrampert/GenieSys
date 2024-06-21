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

std::string ccNames[] = {
        "HI",
        "LS",
        "CC",
        "CS",
        "NE",
        "EQ",
        "VC",
        "VS",
        "PL",
        "MI",
        "GE",
        "LT",
        "GT",
        "LE",
};

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
    if (checkCondition(condition)) {
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
    stream << "B" << ccNames[condition - 2] << " " << std::to_string(displacement);
    return stream.str();
}

bool GenieSys::Bcc::checkCondition(uint8_t condition) {
    auto ccr = cpu->getCcrFlags();
    switch(condition) {
        case BCC_CC:
            return (ccr & CCR_CARRY) == 0;
        case BCC_CS:
            return (ccr & CCR_CARRY) == CCR_CARRY;
        case BCC_EQ:
            return (ccr & CCR_ZERO) == CCR_ZERO;
        case BCC_GE:
            return (ccr & (CCR_NEGATIVE | CCR_OVERFLOW)) == (CCR_NEGATIVE | CCR_OVERFLOW) || (ccr & (CCR_NEGATIVE | CCR_OVERFLOW)) == 0;
        case BCC_GT:
            return (ccr & (CCR_NEGATIVE | CCR_OVERFLOW | CCR_ZERO)) == (CCR_NEGATIVE | CCR_OVERFLOW) || (ccr & (CCR_NEGATIVE | CCR_OVERFLOW | CCR_ZERO)) == 0;
        case BCC_HI:
            return (ccr & (CCR_CARRY | CCR_ZERO)) == 0;
        case BCC_LE:
            return (ccr & CCR_ZERO) == CCR_ZERO || (ccr & (CCR_NEGATIVE | CCR_OVERFLOW)) == CCR_NEGATIVE || (ccr & (CCR_NEGATIVE | CCR_OVERFLOW)) == CCR_OVERFLOW;
        case BCC_LS:
            return (ccr & CCR_CARRY) == CCR_CARRY || (ccr & CCR_ZERO) == CCR_ZERO;
        case BCC_LT:
            return (ccr & (CCR_NEGATIVE | CCR_OVERFLOW)) == CCR_NEGATIVE || (ccr & (CCR_NEGATIVE | CCR_OVERFLOW)) == CCR_OVERFLOW;
        case BCC_MI:
            return (ccr & CCR_NEGATIVE) == CCR_NEGATIVE;
        case BCC_NE:
            return (ccr & CCR_ZERO) == 0;
        case BCC_PL:
            return (ccr & CCR_NEGATIVE) == 0;
        case BCC_VC:
            return (ccr & CCR_OVERFLOW) == 0;
        case BCC_VS:
            return (ccr & CCR_OVERFLOW) == CCR_OVERFLOW;
        default:
            return false;
    }
}
