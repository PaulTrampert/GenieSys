//
// Created by paul.trampert on 11/27/2020.
//
#include <cmath>
#include <GenieSys/signExtend.h>
#include <GenieSys/BriefExtensionWord.h>
#include <GenieSys/ExtensionWord.h>
#include <stdexcept>
#include "GenieSys/AddressingModes/AddressRegisterIndirectWithIndexMode.h"

AddressRegisterIndirectWithIndexMode::AddressRegisterIndirectWithIndexMode(M68kCpu *cpu, Bus *bus)
    : AddressingMode(cpu, bus) {

}

uint32_t AddressRegisterIndirectWithIndexMode::getAddress(uint8_t regAddr) {
    uint16_t extWordRaw = bus->readWord(cpu->getPc());
    cpu->incrementPc(2);
    if ((extWordRaw & 0x0100) > 0) {
        auto extWord = decodeBriefExtensionWord(extWordRaw);
        uint32_t addrReg = cpu->getAddressRegister(regAddr);
        auto displacement = signExtend<int32_t>(extWord.displacement, 8);
        int32_t idxReg = extWord.idxRegType == M68K_REG_TYPE_DATA
                         ? cpu->getDataRegister(extWord.idxRegAddr)
                         : cpu->getAddressRegister(extWord.idxRegAddr);
        if (extWord.idxSize == EXT_WORD_IDX_SIZE_SE_WORD) {
            idxReg = signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
        }
        uint32_t scale = pow(2, extWord.scale);
        return addrReg + displacement + (idxReg * scale);
    }
    else {
        auto extWord = decodeExtensionWord(extWordRaw);
        uint32_t addrReg = extWord.baseRegSuppress ? 0 : cpu->getAddressRegister(regAddr);
        int32_t idxReg = 0;
        if (!extWord.indexSuppress) {
            idxReg = extWord.idxRegType == M68K_REG_TYPE_DATA
                     ? cpu->getDataRegister(extWord.idxRegAddr)
                     : cpu->getAddressRegister(extWord.idxRegAddr);
            if (extWord.idxSize == EXT_WORD_IDX_SIZE_SE_WORD) {
                idxReg = signExtend<int32_t>(idxReg & 0x0000FFFF, 16);
            }
        }
        uint32_t scale = pow(2, extWord.scale);
        int32_t baseDisplacement = 0;
        if (extWord.baseDisplacementSize == EXT_WORD_BD_SIZE_WORD) {
            baseDisplacement = signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
            cpu->incrementPc(2);
        }
        else if (extWord.baseDisplacementSize == EXT_WORD_BD_SIZE_LONG) {
            baseDisplacement = bus->readLong(cpu->getPc());
            cpu->incrementPc(4);
        }
        int32_t outerDisplacement = 0;
        uint32_t intermediateAddr = 0;
        switch(extWord.indexIndirectSelection) {
            case 0:
                // Address Register Indirect Mode
                return addrReg + baseDisplacement + (idxReg * scale);
            case 1:
            case 2:
            case 3:
                // Preindexed
                intermediateAddr = addrReg + baseDisplacement + (idxReg * scale);
                if (extWord.indexIndirectSelection == 2) {
                    outerDisplacement = signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
                    cpu->incrementPc(2);
                }
                else if (extWord.indexIndirectSelection == 3) {
                    outerDisplacement = bus->readLong(cpu->getPc());
                    cpu->incrementPc(4);
                }
                return bus->readLong(intermediateAddr) + outerDisplacement;
            case 5:
            case 6:
            case 7:
                // Postindexed
                intermediateAddr = addrReg + baseDisplacement;
                if (extWord.indexIndirectSelection == 6) {
                    outerDisplacement = signExtend<int32_t>(bus->readWord(cpu->getPc()), 16);
                    cpu->incrementPc(2);
                }
                else if (extWord.indexIndirectSelection == 7) {
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
    return 0b110u;
}
