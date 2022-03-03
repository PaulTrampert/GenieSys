//
// Created by pault on 3/3/2022.
//
#include <GenieSys/CpuOperations/JSR.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/M68kCpu.h>
#include <vector>
#include <sstream>

GenieSys::JSR::JSR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::JSR::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, std::vector<BitMask<uint16_t>*>{&eaModeMask, &eaRegMask});
}

uint8_t GenieSys::JSR::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t GenieSys::JSR::execute(uint16_t opWord) {
    auto eaModeId = eaModeMask.apply(opWord);
    auto eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaData = eaMode->getData(eaReg, 4);
    cpu->stackPush(cpu->getPc());
    cpu->setPc(eaData->getDataAsLong());
    return 8 + eaData->getCycles();
}

std::string GenieSys::JSR::disassemble(uint16_t opWord) {
    auto eaModeId = eaModeMask.apply(opWord);
    auto eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::stringstream stream;
    stream << "JSR " << eaMode->disassemble(eaReg, 4);
    return stream.str();
}
