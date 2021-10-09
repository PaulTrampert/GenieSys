//
// Created by paul.trampert on 12/25/2020.
//

#include <GenieSys/CpuOperations/ANDI.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <sstream>
#include <cmath>



ANDI::ANDI(GenieSys::M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> ANDI::getOpcodes() {
    return getPossibleOpcodes((uint16_t)0b0000001000000000, std::vector<GenieSys::BitMask<uint16_t>*>{
        &sizeMask,
        &eaModeMask,
        &eaRegMask,
    });
}

uint8_t ANDI::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t ANDI::execute(uint16_t opWord) {
    uint8_t size = pow(2, sizeMask.apply(opWord));
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    AddressingMode* immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    auto immData = immMode->getData(ImmediateDataMode::MODE_ID, size);
    AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    auto eaResult = eaMode->getData(eaReg, size);
    bool isMemory = eaModeCode != DataRegisterDirectMode::MODE_ID && eaModeCode != AddressRegisterDirectMode::MODE_ID;
    uint8_t oldCcr = cpu->getCcrFlags();
    uint8_t oldX = oldCcr & CCR_EXTEND;
    uint8_t baseCycles = 1;
    uint8_t byteResult;
    uint16_t wordResult;
    uint32_t longResult;
    switch(size) {
        case 1:
            baseCycles = isMemory ? 12 : 8;
            byteResult = eaResult->getDataAsByte() & immData->getDataAsByte();
            eaResult->write(byteResult);
            cpu->setCcrFlags(oldX | ((int8_t)byteResult < 0 ? CCR_NEGATIVE : 0) | (byteResult == 0 ? CCR_ZERO : 0));
            break;
        case 2:
            baseCycles = isMemory ? 12 : 8;
            wordResult = eaResult->getDataAsWord() & immData->getDataAsWord();
            eaResult->write(wordResult);
            cpu->setCcrFlags(oldX | ((int16_t)wordResult < 0 ? CCR_NEGATIVE : 0) | (wordResult == 0 ? CCR_ZERO : 0));
            break;
        case 4:
            baseCycles = isMemory ? 20 : 16;
            longResult = eaResult->getDataAsLong() & immData->getDataAsLong();
            eaResult->write(longResult);
            cpu->setCcrFlags(oldX | ((int32_t)longResult < 0 ? CCR_NEGATIVE : 0) | (longResult == 0 ? CCR_ZERO : 0));
            break;
        default:
            break;
    }
    return baseCycles + eaResult->getCycles();
}

std::string ANDI::disassemble(uint16_t opWord) {
    uint8_t size = pow(2, sizeMask.apply(opWord));
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    AddressingMode* immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    std::stringstream stream;
    stream << "ANDI " << immMode->disassemble(ImmediateDataMode::MODE_ID, size) << ", " << eaMode->disassemble(eaReg, size);
    return stream.str();
}
