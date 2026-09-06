//
// Created by pault on 10/25/2021.
//
#include <GenieSys/CpuOperations/PEA.h>
#include <vector>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/Bus.h>
#include <GenieSys/getControlModeIndex.h>
#include <GenieSys/enums.h>
#include <sstream>


/**
 * Complete execution time per control addressing mode, indexed by CONTROL_MODE_INDEX.
 * Documented on the PEA row of the JMP, JSR, LEA, PEA, MOVEM instruction timing table.
 */
static const uint8_t CYCLES[GenieSys::CTRL_MODE_COUNT] = {12, 16, 20, 16, 20, 16, 20};

GenieSys::PEA::PEA(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::PEA::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> GenieSys::PEA::getOpcodes() {
    return getPossibleOpcodes(opWordBase, std::vector<BitMask<uint16_t>*> {
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::PEA::execute(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto controlMode = GenieSys::getControlModeIndex(eaModeId, eaReg);
    if (controlMode == GenieSys::CTRL_MODE_INVALID) {
        return cpu->trap(GenieSys::TV_ILLEGAL_INSTR);
    }
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, 4);
    uint32_t address = eaResult->getAddress();
    uint32_t sp = cpu->getAddressRegister(7);
    sp -= 4;
    cpu->setAddressRegister(7, sp);
    bus->writeLong(sp, address);
    return CYCLES[controlMode];
}

std::string GenieSys::PEA::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    stream << "PEA " << eaMode->disassemble(eaReg, 4);
    return stream.str();
}
