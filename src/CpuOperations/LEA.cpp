//
// Created by copilot on 11/24/2024.
//
#include <GenieSys/CpuOperations/LEA.h>
#include <vector>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/Bus.h>
#include <sstream>

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
    // Check for invalid EA modes: 0b000 (Dn), 0b001 (An), 0b011 ((An)+), 0b100 (-(An))
    if (eaModeId == 0b000 || eaModeId == 0b001 || eaModeId == 0b011 || eaModeId == 0b100) {
        return cpu->trap(TV_ILLEGAL_INSTR);
    }
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, 4);
    uint32_t address = eaResult->getAddress();
    cpu->setAddressRegister(destReg, address);
    return 4 + eaResult->getCycles();
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
