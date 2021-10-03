//
// Created by pault on 10/2/2021.
//

#include <GenieSys/CpuOperations/NEGX.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/getCcrFlags.h>
#include <vector>
#include <cmath>

NEGX::NEGX(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t NEGX::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> NEGX::getOpcodes() {
    return getPossibleOpcodes((uint16_t)0b0100000000000000, std::vector<BitMask<uint16_t>*> {
       &sizeMask,
       &eaModeMask,
       &eaRegMask
    });
}

uint8_t NEGX::execute(uint16_t opWord) {
    uint8_t cycles;
    uint8_t sizeBytes = pow(2, sizeMask.apply(opWord));
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, sizeBytes);
    uint8_t oldCcr = cpu->getCcrFlags();
    auto extendBit = (oldCcr & CCR_EXTEND) >> 4;
    switch (sizeBytes) {
        case 1:
            cycles = negxByte(eaResult, oldCcr, extendBit);
            break;
        case 2:
            cycles = negxWord(eaResult, oldCcr, extendBit);
            break;
        case 4:
            cycles = negxLong(eaResult, oldCcr, extendBit);
            break;
        default:
            cycles = 4;
            cpu->trap(TV_ILLEGAL_INSTR);
    }
    return cycles;
}

uint8_t NEGX::negxByte(std::unique_ptr<AddressingResult> &eaResult, uint8_t oldCcr, uint8_t extendBit) {
    uint8_t eaData = eaResult->getDataAsByte();
    uint8_t result = -eaData - extendBit;
    cpu->setCcrFlags(getNegxCcrFlags<uint8_t, int8_t>(result, -eaData, 1, oldCcr));
    eaResult->write(result);
    return 4 + eaResult->getCycles();
}

uint8_t NEGX::negxWord(std::unique_ptr<AddressingResult> &eaResult, uint8_t oldCcr, uint8_t extendBit) {
    uint16_t eaData = eaResult->getDataAsWord();
    uint16_t result = -eaData - extendBit;
    cpu->setCcrFlags(getNegxCcrFlags<uint16_t, int16_t>(result, -eaData, 1, oldCcr));
    eaResult->write(result);
    return 4 + eaResult->getCycles();
}

uint8_t NEGX::negxLong(std::unique_ptr<AddressingResult> &eaResult, uint8_t oldCcr, uint8_t extendBit) {
    uint32_t eaData = eaResult->getDataAsWord();
    uint32_t result = -eaData - extendBit;
    cpu->setCcrFlags(getNegxCcrFlags<uint32_t, int32_t>(result, -eaData, 1, oldCcr));
    eaResult->write(result);
    return 6 + eaResult->getCycles();
}

std::string NEGX::disassemble(uint16_t opWord) {
    return std::string();
}


