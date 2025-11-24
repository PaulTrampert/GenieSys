//
// Created by copilot on 11/24/2024.
//
#include <GenieSys/CpuOperations/LEA.h>
#include <vector>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectWithIndexMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/AbsoluteShortAddressingMode.h>
#include <GenieSys/AddressingModes/AbsoluteLongAddressingMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectWithIndexMode.h>
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
    return getLeaCycles(eaModeId, eaReg);
}

uint8_t GenieSys::LEA::getLeaCycles(uint8_t eaModeId, uint8_t eaReg) {
    // LEA cycle counts based on M68000 Programmer's Reference Manual
    switch (eaModeId) {
        case AddressRegisterIndirectMode::MODE_ID:  // (An) - Address Register Indirect
            return 4;
        case AddressRegisterIndirectDisplacementMode::MODE_ID:  // d(An) - Address Register Indirect with Displacement
            return 8;
        case AddressRegisterIndirectWithIndexMode::MODE_ID:  // d(An,ix) - Address Register Indirect with Index
            return 12;
        case ProgramCounterAddressingMode::MODE_ID:  // PC-relative and Absolute modes
            switch (eaReg) {
                case AbsoluteShortAddressingMode::MODE_ID:  // xxx.W - Absolute Short
                    return 8;
                case AbsoluteLongAddressingMode::MODE_ID:  // xxx.L - Absolute Long
                    return 12;
                case ProgramCounterIndirectDisplacementMode::MODE_ID:  // d(PC) - Program Counter with Displacement
                    return 8;
                case ProgramCounterIndirectWithIndexMode::MODE_ID:  // d(PC,ix) - Program Counter with Index
                    return 12;
                default:
                    return 0;
            }
        default:
            return 0;
    }
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
