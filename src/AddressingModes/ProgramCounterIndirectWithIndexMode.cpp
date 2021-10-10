//
// Created by paul.trampert on 11/27/2020.
//
#include <cmath>
#include <GenieSys/signExtend.h>
#include <stdexcept>
#include <GenieSys/ExtensionWord.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include "GenieSys/AddressingModes/ProgramCounterIndirectWithIndexMode.h"



GenieSys::ProgramCounterIndirectWithIndexMode::ProgramCounterIndirectWithIndexMode(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus)
    : AddressingMode(cpu, bus) {
    cycles = 10;
    longCycles = 14;
}

uint32_t GenieSys::ProgramCounterIndirectWithIndexMode::getAddress(uint8_t regAddr) {
    uint32_t baseAddr = cpu->getPc();
    auto extWord = GenieSys::ExtensionWord(bus->readWord(cpu->getPc()));
    cpu->incrementPc(2);
    uint8_t idxRegAddr = extWord.getIdxRegAddr();
    if (extWord.isBrief()) {
        auto displacement = GenieSys::signExtend<int32_t>(extWord.getDisplacement(), 8);
        int32_t idxReg = extWord.getIdxRegType() == GenieSys::M68K_REG_TYPE_DATA
                         ? cpu->getDataRegister(idxRegAddr)
                         : cpu->getAddressRegister(idxRegAddr);
        if (extWord.getIdxSize() == GenieSys::EXT_WORD_IDX_SIZE_SE_WORD) {
            idxReg = GenieSys::signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
        }
        uint32_t scale = pow(2, extWord.getScale());
        return baseAddr + displacement + (idxReg * scale);
    }
    else {
        baseAddr = extWord.getBaseRegSuppress() ? 0 : baseAddr;
        int32_t idxReg = 0;
        if (!extWord.getIndexSuppress()) {
            idxReg = extWord.getIdxRegType() == GenieSys::M68K_REG_TYPE_DATA
                     ? cpu->getDataRegister(idxRegAddr)
                     : cpu->getAddressRegister(idxRegAddr);
            if (extWord.getIdxSize() == GenieSys::EXT_WORD_IDX_SIZE_SE_WORD) {
                idxReg = GenieSys::signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
            }
        }
        uint32_t scale = pow(2, extWord.getScale());
        int32_t baseDisplacement = 0;
        if (extWord.getBaseDisplacementSize() == GenieSys::EXT_WORD_BD_SIZE_WORD) {
            baseDisplacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
            cpu->incrementPc(2);
        }
        else if (extWord.getBaseDisplacementSize() == GenieSys::EXT_WORD_BD_SIZE_LONG) {
            baseDisplacement = bus->readLong(cpu->getPc());
            cpu->incrementPc(4);
        }
        int32_t outerDisplacement = 0;
        uint32_t intermediateAddr = 0;
        switch(extWord.getIndexIndirectSelection()) {
            case 0:
                // PC Indirect Mode
                return baseAddr + baseDisplacement + (idxReg * scale);
            case 1:
            case 2:
            case 3:
                // Preindexed
                intermediateAddr = baseAddr + baseDisplacement + (idxReg * scale);
                if (extWord.getIndexIndirectSelection() == 2) {
                    outerDisplacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
                    cpu->incrementPc(2);
                }
                else if (extWord.getIndexIndirectSelection() == 3) {
                    outerDisplacement = bus->readLong(cpu->getPc());
                    cpu->incrementPc(4);
                }
                return bus->readLong(intermediateAddr) + outerDisplacement;
            case 5:
            case 6:
            case 7:
                // Postindexed
                intermediateAddr = baseAddr + baseDisplacement;
                if (extWord.getIndexIndirectSelection() == 6) {
                    outerDisplacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
                    cpu->incrementPc(2);
                }
                else if (extWord.getIndexIndirectSelection() == 7) {
                    outerDisplacement = bus->readLong(cpu->getPc());
                    cpu->incrementPc(4);
                }
                return bus->readLong(intermediateAddr) + (idxReg * scale) + outerDisplacement;
            default:
                throw std::runtime_error("Invalid instruction");
        }
    }

}

uint8_t GenieSys::ProgramCounterIndirectWithIndexMode::getModeId() {
    return MODE_ID;
}

std::string GenieSys::ProgramCounterIndirectWithIndexMode::disassemble(uint8_t regAddr, uint8_t size) {
    auto extWord = GenieSys::ExtensionWord(bus->readWord(cpu->getPc()));
    cpu->incrementPc(2);
    uint8_t idxRegAddr = extWord.getIdxRegAddr();
    if (extWord.isBrief()) {
        auto displacement = GenieSys::signExtend<int32_t>(extWord.getDisplacement(), 8);
        auto idxRegType = extWord.getIdxRegType() == GenieSys::M68K_REG_TYPE_DATA ? "D" : "A";
        auto idxRegSize = extWord.getIdxSize() == GenieSys::EXT_WORD_IDX_SIZE_SE_WORD ? ".w" : ".l";
        auto scale = (int)pow(2, extWord.getScale());
        return "(#" + std::to_string(displacement)
               + ",PC"
               + "," + idxRegType + std::to_string(idxRegAddr) + idxRegSize + "*" + std::to_string(scale)
               + ")";
    }
    else {
        std::string baseRegTerm;
        if (!extWord.getBaseRegSuppress()) {
            baseRegTerm = "PC";
        }
        std::string baseDisplacementTerm;
        if (extWord.getBaseDisplacementSize() == GenieSys::EXT_WORD_BD_SIZE_WORD) {
            auto baseDisplacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
            cpu->incrementPc(2);
            baseDisplacementTerm = "#" + std::to_string(baseDisplacement);
        }
        else if (extWord.getBaseDisplacementSize() == GenieSys::EXT_WORD_BD_SIZE_LONG) {
            auto baseDisplacement = bus->readLong(cpu->getPc());
            cpu->incrementPc(4);
            baseDisplacementTerm = "#" + std::to_string(baseDisplacement);
        }
        std::string idxRegTerm;
        if (!extWord.getIndexSuppress()) {
            idxRegTerm += extWord.getIdxRegType() == GenieSys::M68K_REG_TYPE_DATA ? "D" : "A";
            idxRegTerm += std::to_string(extWord.getIdxRegAddr());
            idxRegTerm += extWord.getIdxSize() == GenieSys::EXT_WORD_IDX_SIZE_SE_WORD ? ".w" : ".l";
            auto scale = (int)pow(2, extWord.getScale());
            idxRegTerm += "*" + std::to_string(scale);
        }
        auto idxIndirectSelection = extWord.getIndexIndirectSelection();
        int32_t outerDisplacement;
        std::string outerDisplacementTerm;
        switch(idxIndirectSelection) {
            case 0:
                return "(" + baseDisplacementTerm + "," + baseRegTerm + "," + idxRegTerm + ")";
            case 1:
            case 2:
            case 3:
                // Preindexed
                if (idxIndirectSelection == 2) {
                    outerDisplacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
                    cpu->incrementPc(2);
                }
                else if (idxIndirectSelection == 3) {
                    outerDisplacement = bus->readLong(cpu->getPc());
                    cpu->incrementPc(4);
                }
                outerDisplacementTerm = "#" + std::to_string(outerDisplacement);
                return "([" + baseDisplacementTerm + "," + baseRegTerm + "," + idxRegTerm + "]," + outerDisplacementTerm + ")";
            case 5:
            case 6:
            case 7:
                // Preindexed
                if (idxIndirectSelection == 6) {
                    outerDisplacement = GenieSys::signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
                    cpu->incrementPc(2);
                }
                else if (idxIndirectSelection == 7) {
                    outerDisplacement = bus->readLong(cpu->getPc());
                    cpu->incrementPc(4);
                }
                outerDisplacementTerm = "#" + std::to_string(outerDisplacement);
                return "([" + baseDisplacementTerm + "," + baseRegTerm + "]," + idxRegTerm + "," + outerDisplacementTerm + ")";
            default:
                throw std::runtime_error("Invalid instruction");
        }
    }

}

uint8_t GenieSys::ProgramCounterIndirectWithIndexMode::getMoveCycleKey() {
    return 10;
}
