//
// Created by paul.trampert on 12/25/2020.
//

#include <GenieSys/CpuOperations/SUBI.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/getCcrFlags.h>
#include <sstream>
#include <cmath>



SUBI::SUBI(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> SUBI::getOpcodes() {
    return GenieSys::getPossibleOpcodes(BASE_OPCODE, std::vector<GenieSys::BitMask<uint16_t>*>{
        &sizeMask,
        &eaModeMask,
        &eaRegMask,
    });
}

uint8_t SUBI::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t SUBI::execute(uint16_t opWord) {
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
            byteResult = eaResult->getDataAsByte() - immData->getDataAsByte();
            eaResult->write(byteResult);
            newCcrFlags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(byteResult, eaResult->getDataAsByte(),
                                                                            immData->getDataAsByte());
            cpu->setCcrFlags(newCcrFlags);
            break;
        case 2:
            baseCycles = isMemory ? 12 : 8;
            wordResult = eaResult->getDataAsWord() - immData->getDataAsWord();
            eaResult->write(wordResult);
            newCcrFlags = GenieSys::getSubtractionCcrFlags<uint16_t, int16_t>(wordResult, eaResult->getDataAsWord(),
                                                                              immData->getDataAsWord());
            cpu->setCcrFlags(newCcrFlags);
            break;
        case 4:
            baseCycles = isMemory ? 20 : 16;
            longResult = eaResult->getDataAsLong() - immData->getDataAsLong();
            eaResult->write(longResult);
            newCcrFlags = GenieSys::getSubtractionCcrFlags<uint32_t, int32_t>(longResult, eaResult->getDataAsLong(),
                                                                              immData->getDataAsLong());
            cpu->setCcrFlags(newCcrFlags);
            break;
        default:
            break;
    }
    return baseCycles + eaResult->getCycles();
}

std::string SUBI::disassemble(uint16_t opWord) {
    uint8_t size = pow(2, sizeMask.apply(opWord));
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    AddressingMode* immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    std::stringstream stream;
    stream << "SUBI " << immMode->disassemble(ImmediateDataMode::MODE_ID, size) << "," << eaMode->disassemble(eaReg, size);
    return stream.str();
}
