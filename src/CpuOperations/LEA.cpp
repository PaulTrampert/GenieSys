//
// Created by copilot on 11/24/2024.
//
#include <GenieSys/CpuOperations/LEA.h>
#include <vector>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/getControlModeIndex.h>
#include <GenieSys/Bus.h>
#include <sstream>


/**
 * Complete execution time per control addressing mode, indexed by CONTROL_MODE_INDEX.
 * Documented on the LEA row of the JMP, JSR, LEA, PEA, MOVEM instruction timing table.
 */
static const uint8_t CYCLES[GenieSys::CTRL_MODE_COUNT] = {4, 8, 12, 8, 12, 8, 12};

GenieSys::LEA::LEA(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::LEA::getSpecificity() {
    return destRegMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> GenieSys::LEA::getOpcodes() {
    return getPossibleOpcodes(opWordBase, std::vector<BitMask<uint16_t>*> {
        &destRegMask,
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::LEA::execute(uint16_t opWord) {
    uint8_t destReg = destRegMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    // Dn, An, (An)+, -(An) and the immediate sub mode are not control addressing modes.
    auto controlMode = GenieSys::getControlModeIndex(eaModeId, eaReg);
    if (controlMode == GenieSys::CTRL_MODE_INVALID) {
        return cpu->trap(TV_ILLEGAL_INSTR);
    }
    auto eaMode = cpu->getAddressingMode(eaModeId);
    // LEA loads the effective address, so the operand it points at is never read.
    uint32_t address = eaMode->getAddress(eaReg);
    cpu->setAddressRegister(destReg, address);
    return CYCLES[controlMode];
}

std::string GenieSys::LEA::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t destReg = destRegMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    stream << "LEA " << eaMode->disassemble(eaReg, 4) << ",A" << (int)destReg;
    return stream.str();
}
