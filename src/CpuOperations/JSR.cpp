//
// Created by pault on 3/3/2022.
//
#include <GenieSys/CpuOperations/JSR.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/getControlModeIndex.h>
#include <GenieSys/enums.h>
#include <vector>
#include <sstream>


/**
 * Complete execution time per control addressing mode, indexed by CONTROL_MODE_INDEX.
 * Documented on the JSR row of the JMP, JSR, LEA, PEA, MOVEM instruction timing table.
 */
static const uint8_t CYCLES[GenieSys::CTRL_MODE_COUNT] = {16, 18, 22, 18, 20, 18, 22};

GenieSys::JSR::JSR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::JSR::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, std::vector<BitMask<uint16_t>*>{&eaModeMask, &eaRegMask});
}

uint8_t GenieSys::JSR::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t GenieSys::JSR::execute(uint16_t opWord) {
    auto eaModeId = eaModeMask.apply(opWord);
    auto eaReg = eaRegMask.apply(opWord);
    auto controlMode = GenieSys::getControlModeIndex(eaModeId, eaReg);
    if (controlMode == GenieSys::CTRL_MODE_INVALID) {
        return cpu->trap(GenieSys::TV_ILLEGAL_INSTR);
    }
    auto eaMode = cpu->getAddressingMode(eaModeId);
    // A JSR goes to the effective address itself, so the operand is never read. The address is
    // resolved first so that the pushed return address is past any extension words it consumed.
    auto address = eaMode->getAddress(eaReg);
    cpu->stackPushLong(cpu->getPc());
    cpu->setPc(address);
    return CYCLES[controlMode];
}

std::string GenieSys::JSR::disassemble(uint16_t opWord) {
    auto eaModeId = eaModeMask.apply(opWord);
    auto eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::stringstream stream;
    stream << "JSR " << eaMode->disassemble(eaReg, 4);
    return stream.str();
}
