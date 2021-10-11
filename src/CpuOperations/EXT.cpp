//
// Created by pault on 10/10/2021.
//

#include <vector>
#include <sstream>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/signExtend.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/CpuOperations/EXT.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>

std::vector<uint16_t> GenieSys::EXT::getOpcodes() {
    std::vector<uint16_t> btw = getPossibleOpcodes((uint16_t)0b0100100010000000, &regMask);
    std::vector<uint16_t> wtl = getPossibleOpcodes((uint16_t)0b0100100011000000, &regMask);
    std::vector<uint16_t> result(btw.size() + wtl.size());
    result.insert(result.end(), btw.begin(), btw.end());
    result.insert(result.end(), wtl.begin(), wtl.end());
    return result;
}

uint8_t GenieSys::EXT::getSpecificity() {
    return regMask.getWidth();
}

uint8_t GenieSys::EXT::execute(uint16_t opWord) {
    uint8_t reg = regMask.apply(opWord);
    uint8_t opMode = opModeMask.apply(opWord);
    uint32_t regData = cpu->getDataRegister(reg);
    bool negative = false;
    switch(opMode) {
        case CPU_INSTR_EXT_BYTE_TO_WORD:
            regData &= 0x000000FF;
            regData = signExtend<uint16_t>((uint8_t)regData, 8);
            cpu->setDataRegister(reg, (uint16_t)regData);
            negative = (int16_t)regData < 0;
            break;
        case CPU_INSTR_EXT_WORD_TO_LONG:
            regData &= 0x0000FFFF;
            regData = signExtend<uint32_t>((uint8_t)regData, 16);
            cpu->setDataRegister(reg, regData);
            negative = (int32_t)regData < 0;
            break;
        default:
            cpu->trap(TV_ILLEGAL_INSTR);
    }
    uint8_t ccr = cpu->getCcrFlags() & CCR_EXTEND;
    if (regData == 0) {
        ccr |= CCR_ZERO;
    }
    if (negative) {
        ccr |= CCR_NEGATIVE;
    }
    cpu->setCcrFlags(ccr);
    return 4;
}

std::string GenieSys::EXT::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t reg = regMask.apply(opWord);
    uint8_t opMode = opModeMask.apply(opWord);
    auto dataRegMode = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
    stream << "EXT";
    switch(opMode) {
        case CPU_INSTR_EXT_BYTE_TO_WORD:
            stream << ".w " << dataRegMode->disassemble(reg, 2);
            break;
        case CPU_INSTR_EXT_WORD_TO_LONG:
            stream << ".l " << dataRegMode->disassemble(reg, 4);
            break;
        default:
            break;
    }
    return stream.str();
}
