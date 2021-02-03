//
// Created by paul.trampert on 12/25/2020.
//

#include <GenieSys/CpuOperations/ADDI.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <sstream>
#include <cmath>

template<class U, class S>
static uint8_t getCcrFlags(U result, U op1, U op2) {
    uint8_t ccr = 0;
    if (result < op1) {
        ccr |= CCR_OVERFLOW;
    }
    if (((S) result < 0 && (S)op1 > 0 && (S)op2 > 0) ||
        ((S) result > 0 && (S)op1 < 0 && (S)op2 < 0)
            ) {
        ccr |= CCR_EXTEND | CCR_CARRY;
    }
    if (result == 0) {
        ccr |= CCR_ZERO;
    }
    if ((S)result < 0) {
        ccr |= CCR_NEGATIVE;
    }
    return ccr;
}

ADDI::ADDI(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> ADDI::getOpcodes() {
    return getPossibleOpcodes((uint16_t)0b0000010000000000, std::vector<BitMask<uint16_t>*>{
        &sizeMask,
        &eaModeMask,
        &eaRegMask,
    });
}

uint8_t ADDI::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t ADDI::execute(uint16_t opWord) {
    uint8_t size = pow(2, sizeMask.apply(opWord));
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    AddressingMode* immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    auto immData = immMode->getData(ImmediateDataMode::MODE_ID, size);
    AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    auto eaResult = eaMode->getData(eaReg, size);
    bool isMemory = eaModeCode != DataRegisterDirectMode::MODE_ID && eaModeCode != AddressRegisterDirectMode::MODE_ID;
    uint8_t baseCycles = 1;
    uint8_t byteResult;
    uint16_t wordResult;
    uint32_t longResult;
    uint8_t newCcrFlags;
    switch(size) {
        case 1:
            baseCycles = isMemory ? 12 : 8;
            byteResult = eaResult->getDataAsByte() + immData->getDataAsByte();
            eaResult->write(byteResult);
            newCcrFlags = getCcrFlags<uint8_t, int8_t>(byteResult, eaResult->getDataAsByte(), immData->getDataAsByte());
            cpu->setCcrFlags(newCcrFlags);
            break;
        case 2:
            baseCycles = isMemory ? 12 : 8;
            wordResult = eaResult->getDataAsWord() + immData->getDataAsWord();
            eaResult->write(wordResult);
            newCcrFlags = getCcrFlags<uint16_t, int16_t>(wordResult, eaResult->getDataAsWord(), immData->getDataAsWord());
            cpu->setCcrFlags(newCcrFlags);
            break;
        case 4:
            baseCycles = isMemory ? 20 : 16;
            longResult = eaResult->getDataAsLong() + immData->getDataAsLong();
            eaResult->write(longResult);
            newCcrFlags = getCcrFlags<uint32_t, int32_t>(longResult, eaResult->getDataAsLong(), immData->getDataAsLong());
            cpu->setCcrFlags(newCcrFlags);
            break;
        default:
            break;
    }
    return baseCycles + eaResult->getCycles();
}

std::string ADDI::disassemble(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    AddressingMode* immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    std::stringstream stream;
    stream << "ADDI " << immMode->disassemble(ImmediateDataMode::MODE_ID, size) << "," << eaMode->disassemble(eaReg, size);
    return stream.str();
}
