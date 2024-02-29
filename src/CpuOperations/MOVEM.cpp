//
// Created by pault on 5/3/2022.
//
#include <GenieSys/CpuOperations/MOVEM.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPostIncrementMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectWithIndexMode.h>
#include <GenieSys/AddressingModes/AbsoluteLongAddressingMode.h>
#include <GenieSys/AddressingModes/AbsoluteShortAddressingMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectWithIndexMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <vector>
#include <algorithm>
#include "GenieSys/TrapException.h"


#define DIR_REG_TO_MEM 0
#define DIR_MEM_TO_REG 1
#define SZ_WORD 0
#define SZ_LONG 1

using namespace GenieSys;

GenieSys::MOVEM::MOVEM(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {
}

std::vector<uint16_t> GenieSys::MOVEM::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, std::vector<BitMask<uint16_t>*> {
        &dirMask,
        &sizeMask,
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::MOVEM::getSpecificity() {
    return dirMask.getWidth() + sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t GenieSys::MOVEM::execute(uint16_t opWord) {
    uint8_t dir = dirMask.apply(opWord);
    uint8_t size = sizeMask.apply(opWord) == SZ_WORD ? 2 : 4;
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    uint16_t regListWord = bus->readWord(cpu->getPc());
    cpu->incrementPc(2);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::unique_ptr<AddressingResult> result;
    try {
        if (dir == DIR_MEM_TO_REG) {
            result = eaMode->movemToReg(eaReg, size, regListWord);
        }
        else {
            result = eaMode->movemToMem(eaReg, size, regListWord);
        }
    } catch (TrapException &e) {
        return cpu->trap(e.getTrapVector());
    }
    return result->getCycles();
}

std::string MOVEM::disassemble(uint16_t opWord) {
    return std::string();
}
