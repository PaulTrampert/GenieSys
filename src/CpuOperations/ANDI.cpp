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

ANDI::ANDI(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> ANDI::getOpcodes() {
    return getPossibleOpcodes((uint16_t)0b0000001000000000, std::vector<BitMask<uint16_t>*>{
        &sizeMask,
        &eaModeMask,
        &eaRegMask,
    });
}

uint8_t ANDI::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t ANDI::execute(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    AddressingMode* immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    auto immData = immMode->getData(ImmediateDataMode::MODE_ID, size);
    AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    auto eaResult = eaMode->getData(eaReg, size);
    bool isMemory = eaModeCode != DataRegisterDirectMode::MODE_ID && eaModeCode != AddressRegisterDirectMode::MODE_ID;
    uint8_t baseCycles = 1;
    switch(size) {
        case 1:
            baseCycles = isMemory ? 12 : 8;
            eaResult->write((uint8_t)(eaResult->getDataAsByte() & immData->getDataAsByte()));
            break;
        case 2:
            baseCycles = isMemory ? 12 : 8;
            eaResult->write((uint16_t)(eaResult->getDataAsWord() & immData->getDataAsWord()));
            break;
        case 4:
            baseCycles = isMemory ? 20 : 16;
            eaResult->write((uint32_t)(eaResult->getDataAsLong() & immData->getDataAsLong()));
            break;
        default:
            break;
    }
    return baseCycles + eaResult->getCycles();
}

std::string ANDI::disassemble(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    AddressingMode* immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    std::stringstream stream;
    stream << "ANDI " << immMode->disassemble(ImmediateDataMode::MODE_ID, size) << "," << eaMode->disassemble(eaReg, size);
    return stream.str();
}
