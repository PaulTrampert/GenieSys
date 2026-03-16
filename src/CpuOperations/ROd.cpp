//
// Created by copilot
//

#include <GenieSys/CpuOperations/ROd.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <vector>
#include <cmath>
#include <sstream>


GenieSys::ROd::ROd(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {
}

uint8_t GenieSys::ROd::getSpecificity() {
    return countRegMask.getWidth() + directionMask.getWidth() + sizeMask.getWidth()
           + irMask.getWidth() + destRegMask.getWidth() + eaModeMask.getWidth();
}

std::vector<uint16_t> GenieSys::ROd::getOpcodes() {
    // Register form base: 1110 0000 0001 1000 = 0xE018
    // Bits 15-12=1110 (shift/rotate), bits 4-3=11 (ROd type), all variable bits at 0
    // sizeMask is restricted to 0-2 (byte/word/long) so size=3 (memory indicator) is not generated
    auto regOpcodes = GenieSys::getPossibleOpcodes(
        (uint16_t)0xE018,
        std::vector<GenieSys::BitMask<uint16_t>*>{
            &countRegMask,
            &directionMask,
            &sizeMask,
            &irMask,
            &destRegMask
        });

    // Memory form base: 1110 0110 1100 0000 = 0xE6C0
    // Bits 15-12=1110, bits 11-9=011 (ROd), bits 7-6=11 (memory form)
    auto memOpcodes = GenieSys::getPossibleOpcodes(
        (uint16_t)0xE6C0,
        std::vector<GenieSys::BitMask<uint16_t>*>{
            &directionMask,
            &eaModeMask,
            &eaRegMask
        });

    regOpcodes.insert(regOpcodes.end(), memOpcodes.begin(), memOpcodes.end());
    return regOpcodes;
}

uint8_t GenieSys::ROd::execute(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    if (size == 3) {
        return executeMemory(opWord);
    }
    return executeRegister(opWord, size);
}

uint8_t GenieSys::ROd::executeRegister(uint16_t opWord, uint8_t size) {
    uint8_t countRegField = countRegMask.apply(opWord);
    bool left = directionMask.apply(opWord) == 1;
    bool useRegister = irMask.apply(opWord) == 1;
    uint8_t destReg = destRegMask.apply(opWord);

    uint32_t effectiveCount;
    if (useRegister) {
        effectiveCount = cpu->getDataRegister(countRegField) % 64;
    } else {
        // Immediate: field 000 encodes count of 8; 001-111 encode 1-7
        effectiveCount = (countRegField == 0) ? 8 : countRegField;
    }

    uint32_t regVal = cpu->getDataRegister(destReg);

    switch (size) {
        case 0: { // byte
            uint8_t val = static_cast<uint8_t>(regVal);
            uint8_t res = left ? doRotateLeft<uint8_t>(val, effectiveCount)
                               : doRotateRight<uint8_t>(val, effectiveCount);
            bool carry = left ? carryROL<uint8_t>(res, effectiveCount)
                              : carryROR<uint8_t>(res, effectiveCount);
            cpu->setDataRegister(destReg, (regVal & 0xFFFFFF00u) | res);
            cpu->setCcrFlags(computeCcr<uint8_t, int8_t>(res, carry));
            return static_cast<uint8_t>(6 + 2 * effectiveCount);
        }
        case 1: { // word
            uint16_t val = static_cast<uint16_t>(regVal);
            uint16_t res = left ? doRotateLeft<uint16_t>(val, effectiveCount)
                                : doRotateRight<uint16_t>(val, effectiveCount);
            bool carry = left ? carryROL<uint16_t>(res, effectiveCount)
                              : carryROR<uint16_t>(res, effectiveCount);
            cpu->setDataRegister(destReg, (regVal & 0xFFFF0000u) | res);
            cpu->setCcrFlags(computeCcr<uint16_t, int16_t>(res, carry));
            return static_cast<uint8_t>(6 + 2 * effectiveCount);
        }
        case 2: { // long
            uint32_t val = regVal;
            uint32_t res = left ? doRotateLeft<uint32_t>(val, effectiveCount)
                                : doRotateRight<uint32_t>(val, effectiveCount);
            bool carry = left ? carryROL<uint32_t>(res, effectiveCount)
                              : carryROR<uint32_t>(res, effectiveCount);
            cpu->setDataRegister(destReg, res);
            cpu->setCcrFlags(computeCcr<uint32_t, int32_t>(res, carry));
            return static_cast<uint8_t>(8 + 2 * effectiveCount);
        }
        default:
            return cpu->trap(GenieSys::TV_ILLEGAL_INSTR);
    }
}

uint8_t GenieSys::ROd::executeMemory(uint16_t opWord) {
    bool left = directionMask.apply(opWord) == 1;
    uint8_t eaMode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);

    auto mode = cpu->getAddressingMode(eaMode);
    auto eaResult = mode->getData(eaReg, 2); // word only, 1-bit rotate

    uint16_t val = eaResult->getDataAsWord();
    uint16_t res;
    bool carry;

    if (left) {
        res = doRotateLeft<uint16_t>(val, 1);
        carry = carryROL<uint16_t>(res, 1);
    } else {
        res = doRotateRight<uint16_t>(val, 1);
        carry = carryROR<uint16_t>(res, 1);
    }

    eaResult->write(res);
    cpu->setCcrFlags(computeCcr<uint16_t, int16_t>(res, carry));
    return static_cast<uint8_t>(8 + eaResult->getCycles());
}

std::string GenieSys::ROd::disassemble(uint16_t opWord) {
    bool left = directionMask.apply(opWord) == 1;
    std::string mnemonic = left ? "ROL" : "ROR";
    uint8_t size = sizeMask.apply(opWord);

    std::stringstream stream;

    if (size == 3) {
        // Memory form: ROL.w <ea> or ROR.w <ea>
        uint8_t eaMode = eaModeMask.apply(opWord);
        uint8_t eaReg = eaRegMask.apply(opWord);
        auto mode = cpu->getAddressingMode(eaMode);
        stream << mnemonic << ".w " << mode->disassemble(eaReg, 2);
    } else {
        // Register form
        uint8_t countRegField = countRegMask.apply(opWord);
        bool useRegister = irMask.apply(opWord) == 1;
        uint8_t destReg = destRegMask.apply(opWord);
        uint8_t sizeBytes = static_cast<uint8_t>(pow(2, size));

        stream << mnemonic;
        switch (size) {
            case 0: stream << ".b "; break;
            case 1: stream << ".w "; break;
            case 2: stream << ".l "; break;
            default: stream << " "; break;
        }

        auto dn = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
        if (useRegister) {
            stream << dn->disassemble(countRegField, 4) << "," << dn->disassemble(destReg, sizeBytes);
        } else {
            uint8_t count = (countRegField == 0) ? 8 : countRegField;
            stream << "#" << static_cast<int>(count) << "," << dn->disassemble(destReg, sizeBytes);
        }
    }

    return stream.str();
}
