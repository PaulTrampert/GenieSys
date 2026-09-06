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
#include <bit>
#include <vector>
#include <algorithm>
#include <sstream>
#include "GenieSys/TrapException.h"
#include "GenieSys/numberUtils.h"
#include "GenieSys/RegisterList.h"


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
    return getMovemCycles(dir, size, eaModeId, eaReg, std::popcount(regListWord));
}

/**
 * Documented on the MOVEM row of the JMP, JSR, LEA, PEA, MOVEM instruction timing table:
 * a per addressing mode base plus 4 cycles per word, or 8 cycles per long word, transferred.
 * The base already covers the effective address calculation, so the addressing mode's own
 * cycle count is not added on top.
 */
uint8_t GenieSys::MOVEM::getMovemCycles(uint8_t dir, uint8_t size, uint8_t eaModeId, uint8_t eaReg, uint8_t regCount) {
    uint8_t base;
    bool memToReg = dir == DIR_MEM_TO_REG;
    switch (eaModeId) {
        case AddressRegisterIndirectMode::MODE_ID:                  // (An)
        case AddressRegisterIndirectPostIncrementMode::MODE_ID:     // (An)+, memory to register only
        case AddressRegisterIndirectPreDecrementMode::MODE_ID:      // -(An), register to memory only
            base = memToReg ? 12 : 8;
            break;
        case AddressRegisterIndirectDisplacementMode::MODE_ID:      // d(An)
            base = memToReg ? 16 : 12;
            break;
        case AddressRegisterIndirectWithIndexMode::MODE_ID:         // d(An,Xn)
            base = memToReg ? 18 : 14;
            break;
        case ProgramCounterAddressingMode::MODE_ID:
            switch (eaReg) {
                case AbsoluteShortAddressingMode::MODE_ID:          // (xxx).W
                    base = memToReg ? 16 : 12;
                    break;
                case AbsoluteLongAddressingMode::MODE_ID:           // (xxx).L
                    base = memToReg ? 20 : 16;
                    break;
                case ProgramCounterIndirectDisplacementMode::MODE_ID:   // d(PC), memory to register only
                    base = 16;
                    break;
                case ProgramCounterIndirectWithIndexMode::MODE_ID:      // d(PC,Xn), memory to register only
                    base = 18;
                    break;
                default:
                    base = memToReg ? 12 : 8;
                    break;
            }
            break;
        default:
            // Every remaining mode is illegal for MOVEM and has already trapped out of execute.
            base = memToReg ? 12 : 8;
            break;
    }
    return base + (size == 2 ? 4 : 8) * regCount;
}

std::string MOVEM::disassemble(uint16_t opWord) {
    uint8_t dir = dirMask.apply(opWord);
    uint8_t size = sizeMask.apply(opWord) == SZ_WORD ? 2 : 4;
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto regListWord = bus->readWord(cpu->getPc());
    cpu->incrementPc(2);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::stringstream stream;
    stream << "MOVEM";
    if (size == 2) {
        stream << ".W ";
    }
    else {
        stream << ".L ";
    }

    auto regList = GenieSys::RegisterList(regListWord, eaModeId == AddressRegisterIndirectPreDecrementMode::MODE_ID);

    if (dir == DIR_MEM_TO_REG) {
        stream << eaMode->disassemble(eaReg, size) << "," << regList.toString() << "";
    }
    else {
        stream << regList.toString() << "," << eaMode->disassemble(eaReg, size);
    }
    return stream.str();
}
