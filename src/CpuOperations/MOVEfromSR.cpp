//
// Created by pault on 9/26/2021.
//

#include <GenieSys/CpuOperations/MOVEfromSR.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <vector>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <sstream>


MOVEfromSR::MOVEfromSR(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> MOVEfromSR::getOpcodes() {
    return getPossibleOpcodes((uint16_t)0b0100001011000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t MOVEfromSR::execute(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaResult = eaMode->getData(eaReg, 2);
    eaResult->write(cpu->getSR());
    return (eaModeId == DataRegisterDirectMode::MODE_ID ? 6 : 8) + eaResult->getCycles();
}

std::string MOVEfromSR::disassemble(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint8_t eaReg = eaRegMask.apply(opWord);
    std::stringstream stream;
    stream << "MOVE SR," << eaMode->disassemble(eaReg, 2);
    return stream.str();
}

uint8_t MOVEfromSR::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}
