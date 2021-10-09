//
// Created by pault on 10/4/2021.
//

#include <GenieSys/CpuOperations/CLR.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <vector>
#include <sstream>
#include <cmath>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>



CLR::CLR(GenieSys::M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t CLR::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> CLR::getOpcodes() {
    return GenieSys::getPossibleOpcodes((uint16_t)0b0100001000000000u, std::vector<GenieSys::BitMask<uint16_t>*> {
        &sizeMask,
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t CLR::execute(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t sizeBytes = pow(2, size);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, sizeBytes);
    std::vector<uint8_t> zeros(sizeBytes, 0);
    eaResult->write(zeros);
    auto initCcrX = cpu->getCcrFlags() & GenieSys::CCR_EXTEND;
    cpu->setCcrFlags(initCcrX | GenieSys::CCR_ZERO);
    bool isRegisterEa = eaModeId == DataRegisterDirectMode::MODE_ID || eaModeId == AddressRegisterDirectMode::MODE_ID;
    switch(size) {
        case 0:
        case 1:
            return isRegisterEa ? 4 : (8 + eaResult->getCycles());
        case 2:
        default:
            return isRegisterEa ? 6 : (12 + eaResult->getCycles());
    }
}

std::string CLR::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t size = sizeMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    stream << "CLR";
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

