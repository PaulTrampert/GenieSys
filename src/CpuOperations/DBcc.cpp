//
// Created by paul on 5/2/25.
//
#include <sstream>
#include "GenieSys/CpuOperations/DBcc.h"
#include "GenieSys/getPossibleOpcodes.h"
#include "GenieSys/signExtend.h"
#include "GenieSys/M68kCpu.h"
#include "GenieSys/Bus.h"
#include "GenieSys/ConditionCodes.h"
#include "GenieSys/numberUtils.h"

GenieSys::DBcc::DBcc(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::DBcc::getSpecificity() {
    return conditionMask.getWidth() + regMask.getWidth();
}

std::vector<uint16_t> GenieSys::DBcc::getOpcodes() {
    return getPossibleOpcodes(BASE_OPCODE, std::vector<GenieSys::BitMask<uint16_t>*> {
        &conditionMask,
        &regMask
    });
}

uint8_t GenieSys::DBcc::execute(uint16_t opWord) {
    uint8_t condition = conditionMask.apply(opWord);
    uint8_t reg = regMask.apply(opWord);
    uint16_t data = cpu->getDataRegister(reg) & 0x0000FFFF;
    auto displacement = signExtend<uint32_t>(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    uint8_t cycles = 12;
    if (!cpu->testConditionCode(condition)) {
        cycles = 10;
        data--;
        cpu->setDataRegister(reg, data);
        if ((int16_t)data != -1) {
            cycles = 14;
            cpu->setPc(cpu->getPc() + displacement);
        }
    }
    return cycles;
}

std::string GenieSys::DBcc::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t condition = conditionMask.apply(opWord);
    uint8_t reg = regMask.apply(opWord);
    auto displacement = bus->read(cpu->getPc(), 2);
    cpu->incrementPc(2);
    stream << "DB" << getConditionCodeName(condition) << " D" << (int)reg << ", $" << GenieSys::toHex(displacement);
    return stream.str();
}
