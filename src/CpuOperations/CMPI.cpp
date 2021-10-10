//
// Created by paul.trampert on 12/25/2020.
//

#include <GenieSys/CpuOperations/CMPI.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/getCcrFlags.h>
#include <sstream>
#include <cmath>



GenieSys::CMPI::CMPI(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::CMPI::getOpcodes() {
    return GenieSys::getPossibleOpcodes(BASE_OPCODE, std::vector<GenieSys::BitMask<uint16_t>*>{
        &sizeMask,
        &eaModeMask,
        &eaRegMask,
    });
}

uint8_t GenieSys::CMPI::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t GenieSys::CMPI::execute(uint16_t opWord) {
    uint8_t size = pow(2, sizeMask.apply(opWord));
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    GenieSys::AddressingMode* immMode = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
    auto immData = immMode->getData(GenieSys::ImmediateDataMode::MODE_ID, size);
    GenieSys::AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    auto eaResult = eaMode->getData(eaReg, size);
    bool isMemory = eaModeCode != GenieSys::DataRegisterDirectMode::MODE_ID && eaModeCode != GenieSys::AddressRegisterDirectMode::MODE_ID;
    uint8_t baseCycles = 1;
    uint8_t byteResult;
    uint16_t wordResult;
    uint32_t longResult;
    uint8_t newCcrFlags;
    switch(size) {
        case 1:
            baseCycles = isMemory ? 8 : 8;
            byteResult = eaResult->getDataAsByte() - immData->getDataAsByte();
            newCcrFlags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(byteResult, eaResult->getDataAsByte(),
                                                                            immData->getDataAsByte());
            cpu->setCcrFlags(newCcrFlags);
            break;
        case 2:
            baseCycles = isMemory ? 8 : 8;
            wordResult = eaResult->getDataAsWord() - immData->getDataAsWord();
            newCcrFlags = GenieSys::getSubtractionCcrFlags<uint16_t, int16_t>(wordResult, eaResult->getDataAsWord(),
                                                                              immData->getDataAsWord());
            cpu->setCcrFlags(newCcrFlags);
            break;
        case 4:
            baseCycles = isMemory ? 12 : 14;
            longResult = eaResult->getDataAsLong() - immData->getDataAsLong();
            newCcrFlags = GenieSys::getSubtractionCcrFlags<uint32_t, int32_t>(longResult, eaResult->getDataAsLong(),
                                                                              immData->getDataAsLong());
            cpu->setCcrFlags(newCcrFlags);
            break;
        default:
            break;
    }
    return baseCycles + eaResult->getCycles();
}

std::string GenieSys::CMPI::disassemble(uint16_t opWord) {
    uint8_t size = pow(2, sizeMask.apply(opWord));
    uint8_t eaModeCode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    GenieSys::AddressingMode* immMode = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
    GenieSys::AddressingMode* eaMode = cpu->getAddressingMode(eaModeCode);
    std::stringstream stream;
    stream << "CMPI " << immMode->disassemble(GenieSys::ImmediateDataMode::MODE_ID, size) << "," << eaMode->disassemble(eaReg, size);
    return stream.str();
}
