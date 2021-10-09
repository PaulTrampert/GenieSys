//
// Created by pault on 10/2/2021.
//

#include <GenieSys/CpuOperations/NEGX.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/getCcrFlags.h>
#include <vector>
#include <cmath>
#include <sstream>



NEGX::NEGX(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t NEGX::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> NEGX::getOpcodes() {
    return GenieSys::getPossibleOpcodes((uint16_t)0b0100000000000000, std::vector<GenieSys::BitMask<uint16_t>*> {
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
    auto extendBit = (oldCcr & GenieSys::CCR_EXTEND) >> 4;
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
            cpu->trap(GenieSys::TV_ILLEGAL_INSTR);
    }
    return cycles;
}

uint8_t NEGX::negxByte(std::unique_ptr<AddressingResult> &eaResult, uint8_t oldCcr, uint8_t extendBit) {
    auto eaData = eaResult->getDataAsByte();
    uint8_t result = -eaData - extendBit;
    cpu->setCcrFlags(GenieSys::getNegxCcrFlags<uint8_t, int8_t>(result, -eaData, extendBit, oldCcr));
    eaResult->write((uint8_t)result);
    return 4 + eaResult->getCycles();
}

uint8_t NEGX::negxWord(std::unique_ptr<AddressingResult> &eaResult, uint8_t oldCcr, uint8_t extendBit) {
    auto eaData = eaResult->getDataAsWord();
    uint16_t result = -eaData - extendBit;
    cpu->setCcrFlags(GenieSys::getNegxCcrFlags<uint16_t, int16_t>(result, -eaData, extendBit, oldCcr));
    eaResult->write((uint16_t)result);
    return 4 + eaResult->getCycles();
}

uint8_t NEGX::negxLong(std::unique_ptr<AddressingResult> &eaResult, uint8_t oldCcr, uint8_t extendBit) {
    auto eaData = eaResult->getDataAsLong();
    uint32_t result = -eaData - extendBit;
    cpu->setCcrFlags(GenieSys::getNegxCcrFlags<uint32_t, int32_t>(result, -eaData, extendBit, oldCcr));
    eaResult->write((uint32_t)result);
    return 6 + eaResult->getCycles();
}

std::string NEGX::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t size = sizeMask.apply(opWord);
    uint8_t sizeBytes = pow(2, size);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    std::string sizeStr;
    switch(size) {
        case 0:
            sizeStr = ".b";
            break;
        case 1:
            sizeStr = ".w";
            break;
        case 2:
            sizeStr = ".l";
            break;
        default:
            sizeStr = "";
            break;
    }
    auto eaMode = cpu->getAddressingMode(eaModeId);
    stream << "NEGX" << sizeStr << " " << eaMode->disassemble(eaReg, sizeBytes);
    return stream.str();
}


