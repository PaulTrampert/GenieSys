//
// Created by pault on 3/24/2022.
//

#include <GenieSys/CpuOperations/JMP.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/getControlModeIndex.h>
#include <GenieSys/enums.h>
#include <vector>
#include <sstream>


/**
 * Complete execution time per control addressing mode, indexed by CONTROL_MODE_INDEX.
 * Documented on the JMP row of the JMP, JSR, LEA, PEA, MOVEM instruction timing table.
 */
static const uint8_t CYCLES[GenieSys::CTRL_MODE_COUNT] = {8, 10, 14, 10, 12, 10, 14};

GenieSys::JMP::JMP(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::JMP::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, std::vector<BitMask<uint16_t>*>{
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::JMP::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t GenieSys::JMP::execute(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto controlMode = GenieSys::getControlModeIndex(eaModeId, eaReg);
    if (controlMode == GenieSys::CTRL_MODE_INVALID) {
        return cpu->trap(GenieSys::TV_ILLEGAL_INSTR);
    }
    auto eaMode = cpu->getAddressingMode(eaModeId);
    // A JMP goes to the effective address itself, so the operand is never read.
    cpu->setPc(eaMode->getAddress(eaReg));
    return CYCLES[controlMode];
}

std::string GenieSys::JMP::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    stream << "JMP " << eaMode->disassemble(eaReg, 4);
    return stream.str();
}
