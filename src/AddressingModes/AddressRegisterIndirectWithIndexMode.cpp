//
// Created by paul.trampert on 11/27/2020.
//
#include <cmath>
#include <GenieSys/signExtend.h>
#include <GenieSys/ExtensionWord.h>
#include <stdexcept>
#include "GenieSys/AddressingModes/AddressRegisterIndirectWithIndexMode.h"

AddressRegisterIndirectWithIndexMode::AddressRegisterIndirectWithIndexMode(M68kCpu *cpu, Bus *bus)
    : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterIndirectWithIndexMode::getAddress(uint8_t regAddr) {
    auto extWord = ExtensionWord(bus->readWord(cpu->getPc()));
    cpu->incrementPc(2);
    uint8_t idxRegAddr = extWord.getIdxRegAddr();
    if (extWord.isBrief()) {
        uint32_t addrReg = cpu->getAddressRegister(regAddr);
        auto displacement = signExtend<int32_t>(extWord.getDisplacement(), 8);
        int32_t idxReg = extWord.getIdxRegType() == M68K_REG_TYPE_DATA
                         ? cpu->getDataRegister(idxRegAddr)
                         : cpu->getAddressRegister(idxRegAddr);
        if (extWord.getIdxSize() == EXT_WORD_IDX_SIZE_SE_WORD) {
            idxReg = signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
        }
        uint32_t scale = pow(2, extWord.getScale());
        return addrReg + displacement + (idxReg * scale);
    }
    else {
        uint32_t addrReg = extWord.getBaseRegSuppress() ? 0 : cpu->getAddressRegister(regAddr);
        int32_t idxReg = 0;
        if (!extWord.getIndexSuppress()) {
            idxReg = extWord.getIdxRegType() == M68K_REG_TYPE_DATA
                     ? cpu->getDataRegister(idxRegAddr)
                     : cpu->getAddressRegister(idxRegAddr);
            if (extWord.getIdxSize() == EXT_WORD_IDX_SIZE_SE_WORD) {
                idxReg = signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
            }
        }
        uint32_t scale = pow(2, extWord.getScale());
        int32_t baseDisplacement = 0;
        if (extWord.getBaseDisplacementSize() == EXT_WORD_BD_SIZE_WORD) {
            baseDisplacement = signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
            cpu->incrementPc(2);
        }
        else if (extWord.getBaseDisplacementSize() == EXT_WORD_BD_SIZE_LONG) {
            baseDisplacement = bus->readLong(cpu->getPc());
            cpu->incrementPc(4);
        }
        int32_t outerDisplacement = 0;
        uint32_t intermediateAddr;
        uint8_t idxIndirectSelection = extWord.getIndexIndirectSelection();
        switch(idxIndirectSelection) {
            case 0:
                // Address Register Indirect Mode
                return addrReg + baseDisplacement + (idxReg * scale);
            case 1:
            case 2:
            case 3:
                // Preindexed
                intermediateAddr = addrReg + baseDisplacement + (idxReg * scale);
                if (idxIndirectSelection == 2) {
                    outerDisplacement = signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
                    cpu->incrementPc(2);
                }
                else if (idxIndirectSelection == 3) {
                    outerDisplacement = bus->readLong(cpu->getPc());
                    cpu->incrementPc(4);
                }
                return bus->readLong(intermediateAddr) + outerDisplacement;
            case 5:
            case 6:
            case 7:
                // Postindexed
                intermediateAddr = addrReg + baseDisplacement;
                if (idxIndirectSelection == 6) {
                    outerDisplacement = signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
                    cpu->incrementPc(2);
                }
                else if (idxIndirectSelection == 7) {
                    outerDisplacement = bus->readLong(cpu->getPc());
                    cpu->incrementPc(4);
                }
                return bus->readLong(intermediateAddr) + (idxReg * scale) + outerDisplacement;
            default:
                throw std::runtime_error("Invalid instruction");
        }
    }
}

uint8_t AddressRegisterIndirectWithIndexMode::getModeId() {
    return MODE_ID;
}
