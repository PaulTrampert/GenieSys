//
// Created by paul.trampert on 2/24/2021.
//
#include <GenieSys/CpuOperations/ORI.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <sstream>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <cmath>



ORI::ORI(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {

}

std::string ORI::disassemble(uint16_t opWord) {
    uint8_t size = pow(2, sizeMask.apply(opWord));
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    GenieSys::AddressingMode* immMode = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
    GenieSys::AddressingMode* eaMode = cpu->getAddressingMode(eaModeId);
    std::stringstream stream;

    stream << "ORI " << immMode->disassemble(GenieSys::ImmediateDataMode::MODE_ID, size) << ", " << eaMode->disassemble(eaReg, size);

    return stream.str();
}

std::vector<uint16_t> ORI::getOpcodes() {
    return GenieSys::getPossibleOpcodes(BASE_OPCODE, std::vector<GenieSys::BitMask<uint16_t>*>{
            &sizeMask,
            &eaModeMask,
            &eaRegMask,
    });
}

uint8_t ORI::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t ORI::execute(uint16_t opWord) {
    uint8_t size = pow(2, sizeMask.apply(opWord));
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    GenieSys::AddressingMode* immMode = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
    auto immData = immMode->getData(GenieSys::ImmediateDataMode::MODE_ID, size);
    GenieSys::AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    auto eaResult = eaMode->getData(eaReg, size);
    bool isMemory = eaModeCode != GenieSys::DataRegisterDirectMode::MODE_ID && eaModeCode != GenieSys::AddressRegisterDirectMode::MODE_ID;
    uint8_t oldCcr = cpu->getCcrFlags();
    uint8_t oldX = oldCcr & GenieSys::CCR_EXTEND;
    uint8_t baseCycles = 1;
    uint8_t byteResult;
    uint16_t wordResult;
    uint32_t longResult;
    switch(size) {
        case 1:
            baseCycles = isMemory ? 12 : 8;
            byteResult = eaResult->getDataAsByte() | immData->getDataAsByte();
            eaResult->write(byteResult);
            cpu->setCcrFlags(oldX | ((int8_t)byteResult < 0 ? GenieSys::CCR_NEGATIVE : 0) | (byteResult == 0 ? GenieSys::CCR_ZERO : 0));
            break;
        case 2:
            baseCycles = isMemory ? 12 : 8;
            wordResult = eaResult->getDataAsWord() | immData->getDataAsWord();
            eaResult->write(wordResult);
            cpu->setCcrFlags(oldX | ((int16_t)wordResult < 0 ? GenieSys::CCR_NEGATIVE : 0) | (wordResult == 0 ? GenieSys::CCR_ZERO : 0));
            break;
        case 4:
            baseCycles = isMemory ? 20 : 16;
            longResult = eaResult->getDataAsLong() | immData->getDataAsLong();
            eaResult->write(longResult);
            cpu->setCcrFlags(oldX | ((int32_t)longResult < 0 ? GenieSys::CCR_NEGATIVE : 0) | (longResult == 0 ? GenieSys::CCR_ZERO : 0));
            break;
        default:
            break;
    }
    return baseCycles + eaResult->getCycles();
}