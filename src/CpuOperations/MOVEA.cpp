//
// Created by pault on 9/18/2021.
//

#include <GenieSys/CpuOperations/MOVEA.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/signExtend.h>
#include <cmath>
#include <sstream>



MOVEA::MOVEA(GenieSys::M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t MOVEA::getSpecificity() {
    return sizeMask.getWidth() + destMask.getWidth() + eaModeMask.getWidth() + eaAddrMask.getWidth();
}

std::vector<uint16_t> MOVEA::getOpcodes() {
    return GenieSys::getPossibleOpcodes((uint16_t) 0b0000000001000000, std::vector<GenieSys::BitMask<uint16_t>*> {
       &sizeMask,
       &destMask,
       &eaModeMask,
       &eaAddrMask
    });
}

uint8_t MOVEA::execute(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t sizeBytes = size == 3 ? 2 : 4;
    uint8_t destReg = destMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaAddr = eaAddrMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaAddr, sizeBytes);
    uint32_t regData;
    if (sizeBytes == 2) {
        regData = GenieSys::signExtend<uint32_t>(eaResult->getDataAsWord(), 16);
    }
    else {
        regData = eaResult->getDataAsLong();
    }
    cpu->setAddressRegister(destReg, regData);
    return 4 + eaResult->getCycles();
}

std::string MOVEA::disassemble(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t sizeBytes = size == 3 ? 2 : 4;
    uint8_t destReg = destMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaAddr = eaAddrMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::stringstream stream;
    stream << "MOVEA" << (size == 3 ? ".w " : ".l ") << eaMode->disassemble(eaAddr, sizeBytes) << ",A" << std::to_string(destReg);
    return stream.str();
}
