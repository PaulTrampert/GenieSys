//
// Created by pault on 9/26/2021.
//

#include <GenieSys/CpuOperations/MOVEfromSR.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <vector>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <sstream>



GenieSys::MOVEfromSR::MOVEfromSR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::MOVEfromSR::getOpcodes() {
    return GenieSys::getPossibleOpcodes((uint16_t)0b0100001011000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::MOVEfromSR::execute(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaResult = eaMode->getData(eaReg, 2);
    eaResult->write(cpu->getSR());
    return (eaModeId == GenieSys::DataRegisterDirectMode::MODE_ID ? 6 : 8) + eaResult->getCycles();
}

std::string GenieSys::MOVEfromSR::disassemble(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint8_t eaReg = eaRegMask.apply(opWord);
    std::stringstream stream;
    stream << "MOVE SR," << eaMode->disassemble(eaReg, 2);
    return stream.str();
}

uint8_t GenieSys::MOVEfromSR::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}
