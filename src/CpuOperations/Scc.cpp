//
// Created by paul on 5/2/25.
//
#include <sstream>
#include <GenieSys/CpuOperations/Scc.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include "GenieSys/ConditionCodes.h"
#include "GenieSys/AddressingModes/DataRegisterDirectMode.h"

GenieSys::Scc::Scc(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::Scc::getSpecificity() {
    return conditionMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> GenieSys::Scc::getOpcodes() {
    return getPossibleOpcodes(BASE_OPCODE, std::vector<GenieSys::BitMask<uint16_t>*> {
        &conditionMask,
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::Scc::execute(uint16_t opWord) {
    uint8_t condition = conditionMask.apply(opWord);
    uint8_t eaMode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto addressingMode = cpu->getAddressingMode(eaMode);
    auto eaResult = addressingMode->getData(eaReg, 1);
    uint8_t cycles = 4;
    if (cpu->testConditionCode(condition)) {
        eaResult->write((uint8_t)0xFF);
        cycles = 6;
    } else {
        eaResult->write((uint8_t)0x00);
    }
    if (addressingMode->getModeId() != GenieSys::DataRegisterDirectMode::MODE_ID) {
        cycles = 8;
    }
    return cycles + eaResult->getCycles();
}

std::string GenieSys::Scc::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t condition = conditionMask.apply(opWord);
    uint8_t eaMode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto addressingMode = cpu->getAddressingMode(eaMode);
    stream << "S" << getConditionCodeName(condition) << " " << addressingMode->disassemble(eaReg, 1);
    return stream.str();
}
