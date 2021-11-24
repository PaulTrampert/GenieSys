//
// Created by pault on 9/29/2021.
//

#include <GenieSys/CpuOperations/MOVEtoSR.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <vector>
#include <sstream>
#include <GenieSys/M68kCpu.h>


GenieSys::MOVEtoSR::MOVEtoSR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::MOVEtoSR::getOpcodes() {
    return GenieSys::getPossibleOpcodes((uint16_t) 0b0100011011000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &eaRegMask,
        &eaModeMask
    });
}

uint8_t GenieSys::MOVEtoSR::execute(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaResult = eaMode->getData(eaReg, 2);
    if (!cpu->isSupervisor()) {
        return cpu->trap(GenieSys::TV_PRIVILEGE);
    }
    else {
        cpu->setSR(eaResult->getDataAsWord());
    }
    return 12 + eaResult->getCycles();
}

std::string GenieSys::MOVEtoSR::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t eaModeId = eaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint8_t eaReg = eaRegMask.apply(opWord);
    stream << "MOVE " << eaMode->disassemble(eaReg, 2) << ",SR";
    return stream.str();
}

uint8_t GenieSys::MOVEtoSR::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}
