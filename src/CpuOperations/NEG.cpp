//
// Created by pault on 10/6/2021.
//

#include <GenieSys/CpuOperations/NEG.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/getCcrFlags.h>
#include <vector>
#include <cmath>
#include <sstream>



NEG::NEG(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t NEG::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> NEG::getOpcodes() {
    return getPossibleOpcodes((uint16_t)0b0100010000000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &sizeMask,
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t NEG::execute(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, pow(2, size));
    bool isRegisterEa = eaModeId == DataRegisterDirectMode::MODE_ID || eaModeId == AddressRegisterDirectMode::MODE_ID;
    uint8_t cycles = 4;
    uint8_t byteResult;
    uint16_t wordResult;
    uint32_t longResult;
    switch(size) {
        case 0:
            byteResult = -eaResult->getDataAsByte();
            eaResult->write(byteResult);
            cpu->setCcrFlags(GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(byteResult, 0, -byteResult));
            cycles = isRegisterEa ? 4 : (8 + eaResult->getCycles());
            break;
        case 1:
            wordResult = -eaResult->getDataAsWord();
            eaResult->write(wordResult);
            cpu->setCcrFlags(GenieSys::getSubtractionCcrFlags<uint16_t, int16_t>(wordResult, 0, -wordResult));
            cycles = isRegisterEa ? 4 : (8 + eaResult->getCycles());
            break;
        case 2:
            longResult = -eaResult->getDataAsLong();
            eaResult->write(longResult);
            cpu->setCcrFlags(GenieSys::getSubtractionCcrFlags<uint32_t, int32_t>(longResult, 0, -longResult));
            cycles = isRegisterEa ? 6 : (12 + eaResult->getCycles());
            break;
        default:
            cpu->trap(TV_ILLEGAL_INSTR);
    }
    return cycles;
}

std::string NEG::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t size = sizeMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    stream << "NEG";
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
