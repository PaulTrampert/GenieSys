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


#define DIR_REG_TO_MEM 0
#define DIR_MEM_TO_REG 1
#define SZ_WORD 0
#define SZ_LONG 1

using namespace GenieSys;

GenieSys::MOVEM::MOVEM(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {
    std::vector<uint16_t> tmpRegToMemEas[] = {
            getPossibleOpcodes<uint16_t>(AddressRegisterIndirectMode::MODE_ID, &eaRegMask),
            getPossibleOpcodes<uint16_t>(AddressRegisterIndirectPreDecrementMode::MODE_ID, &eaRegMask),
            getPossibleOpcodes<uint16_t>(AddressRegisterIndirectDisplacementMode::MODE_ID, &eaRegMask),
            getPossibleOpcodes<uint16_t>(AddressRegisterIndirectWithIndexMode::MODE_ID, &eaRegMask),
            std::vector<uint16_t>{(ProgramCounterAddressingMode::MODE_ID << 3) | AbsoluteShortAddressingMode::MODE_ID},
            std::vector<uint16_t>{(ProgramCounterAddressingMode::MODE_ID << 3) | AbsoluteLongAddressingMode::MODE_ID}
    };

    std::vector<uint16_t> tmpMemToRegEas[] = {
            getPossibleOpcodes<uint16_t>(AddressRegisterIndirectMode::MODE_ID, &eaRegMask),
            getPossibleOpcodes<uint16_t>(AddressRegisterIndirectPostIncrementMode::MODE_ID, &eaRegMask),
            getPossibleOpcodes<uint16_t>(AddressRegisterIndirectDisplacementMode::MODE_ID, &eaRegMask),
            getPossibleOpcodes<uint16_t>(AddressRegisterIndirectWithIndexMode::MODE_ID, &eaRegMask),
            std::vector<uint16_t>{(ProgramCounterAddressingMode::MODE_ID << 3) | AbsoluteShortAddressingMode::MODE_ID},
            std::vector<uint16_t>{(ProgramCounterAddressingMode::MODE_ID << 3) | AbsoluteLongAddressingMode::MODE_ID},
            std::vector<uint16_t>{(ProgramCounterAddressingMode::MODE_ID << 3) | ProgramCounterIndirectDisplacementMode::MODE_ID},
            std::vector<uint16_t>{(ProgramCounterAddressingMode::MODE_ID << 3) | ProgramCounterIndirectWithIndexMode::MODE_ID}
    };

    for (auto eas : tmpRegToMemEas) {
        regToMemEas.insert(regToMemEas.end(), eas.begin(), eas.end());
    }
    std::sort(regToMemEas.begin(), regToMemEas.end());

    for (auto eas : tmpMemToRegEas) {
        memToRegEas.insert(memToRegEas.end(), eas.begin(), eas.end());
    }
    std::sort(memToRegEas.begin(), memToRegEas.end());
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
    uint8_t size = sizeMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    uint8_t ea = (eaModeId << 3) | eaReg;
    uint16_t regListWord = bus->readWord(cpu->getPc());
    cpu->incrementPc(2);
    if (dir == DIR_MEM_TO_REG && !std::binary_search(memToRegEas.begin(), memToRegEas.end(), ea)) {
        return cpu->trap(TV_ILLEGAL_INSTR);
    }
    else if (dir == DIR_REG_TO_MEM && !std::binary_search(regToMemEas.begin(), regToMemEas.end(), ea)) {
        return cpu->trap(TV_ILLEGAL_INSTR);
    }
    bool selfIncrMode = eaModeId == AddressRegisterIndirectPostIncrementMode::MODE_ID
            || eaModeId == AddressRegisterIndirectPreDecrementMode::MODE_ID;
    auto eaMode = cpu->getAddressingMode(eaModeId);
    return 0;
}

std::string MOVEM::disassemble(uint16_t opWord) {
    return std::string();
}
