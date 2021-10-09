//
// Created by pault on 10/8/2021.
//

#include <GenieSys/CpuOperations/NOT.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <vector>
#include <cmath>
#include <sstream>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>



NOT::NOT(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t NOT::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> NOT::getOpcodes() {
    return GenieSys::getPossibleOpcodes((uint16_t)0b0100011000000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &sizeMask,
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t NOT::execute(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t sizeBytes = pow(2, size);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, sizeBytes);
    AddressingResult* eaResultPtr = eaResult.get();
    switch(size) {
        case 0:
            return notByte(eaResultPtr, eaModeId);
        case 1:
            return notWord(eaResultPtr, eaModeId);
        case 2:
            return notLong(eaResultPtr, eaModeId);
        default:
            cpu->trap(GenieSys::TV_ILLEGAL_INSTR);
    }
    return 1;
}

uint8_t NOT::notByte(AddressingResult* eaResult, uint8_t eaModeId) {
    uint8_t data = eaResult->getDataAsByte();
    uint8_t result = ~data;
    eaResult->write(result);
    cpu->setCcrFlags(getCcrFlags<uint8_t, int8_t>(result));
    return (eaModeId == DataRegisterDirectMode::MODE_ID || eaModeId == AddressRegisterDirectMode::MODE_ID) ? 4 : (8 + eaResult->getCycles());
}

uint8_t NOT::notWord(AddressingResult *eaResult, uint8_t eaModeId) {
    uint16_t data = eaResult->getDataAsWord();
    uint16_t result = ~data;
    eaResult->write(result);
    cpu->setCcrFlags(getCcrFlags<uint16_t, int16_t>(result));
    return (eaModeId == DataRegisterDirectMode::MODE_ID || eaModeId == AddressRegisterDirectMode::MODE_ID) ? 4 : (8 + eaResult->getCycles());
}

uint8_t NOT::notLong(AddressingResult *eaResult, uint8_t eaModeId) {
    uint32_t data = eaResult->getDataAsLong();
    uint32_t result = ~data;
    eaResult->write(result);
    cpu->setCcrFlags(getCcrFlags<uint32_t, int32_t>(result));
    return (eaModeId == DataRegisterDirectMode::MODE_ID || eaModeId == AddressRegisterDirectMode::MODE_ID) ? 6 : (12 + eaResult->getCycles());
}

std::string NOT::disassemble(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint8_t eaReg = eaRegMask.apply(opWord);
    std::stringstream stream;
    stream << "NOT";
    switch (size) {
        case 0:
            stream << ".b ";
            break;
        case 1:
            stream << ".w ";
            break;
        case 2:
            stream << ".l ";
            break;
        default:
            stream << " ";
    }
    stream << eaMode->disassemble(eaReg, pow(2, size));
    return stream.str();
}
