//
// Created by paul on 11/18/21.
//

#include <GenieSys/CpuOperations/TAS.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include<sstream>
#include <vector>

GenieSys::TAS::TAS(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::TAS::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> GenieSys::TAS::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, std::vector<BitMask<uint16_t>*>{
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::TAS::execute(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, 1);
    uint8_t eaData = eaResult->getDataAsByte();
    auto ccr = cpu->getCcrFlags() & CCR_EXTEND;
    if (eaData == 0)
        ccr |= CCR_ZERO;
    if ((int8_t)eaData < 0)
        ccr |= CCR_NEGATIVE;
    eaResult->write((uint8_t)((1 << 7) | eaData));
    cpu->setCcrFlags(ccr);
    return (eaModeId == DataRegisterDirectMode::MODE_ID || eaModeId == AddressRegisterDirectMode::MODE_ID ? 4 : 8)
        + eaResult->getCycles();
}

std::string GenieSys::TAS::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    stream << "TAS " << eaMode->disassemble(eaReg, 1);
    return stream.str();
}
