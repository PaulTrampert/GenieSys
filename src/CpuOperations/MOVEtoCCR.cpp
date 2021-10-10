//
// Created by pault on 9/29/2021.
//

#include <GenieSys/CpuOperations/MOVEtoCCR.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <vector>
#include <sstream>
#include <GenieSys/M68kCpu.h>


GenieSys::MOVEtoCCR::MOVEtoCCR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::MOVEtoCCR::getOpcodes() {
    return GenieSys::getPossibleOpcodes((uint16_t) 0b0100010011000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &eaRegMask,
        &eaModeMask
    });
}

uint8_t GenieSys::MOVEtoCCR::execute(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaResult = eaMode->getData(eaReg, 2);
    cpu->setCcrFlags(eaResult->getDataAsByte());
    return 12 + eaResult->getCycles();
}

std::string GenieSys::MOVEtoCCR::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t eaModeId = eaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint8_t eaReg = eaRegMask.apply(opWord);
    stream << "MOVE " << eaMode->disassemble(eaReg, 2) << ",CCR";
    return stream.str();
}

uint8_t GenieSys::MOVEtoCCR::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}
