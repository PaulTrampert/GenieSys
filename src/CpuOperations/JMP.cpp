//
// Created by pault on 3/24/2022.
//

#include <GenieSys/CpuOperations/JMP.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/M68kCpu.h>
#include <vector>
#include <sstream>

GenieSys::JMP::JMP(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::JMP::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, std::vector<BitMask<uint16_t>*>{
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::JMP::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t GenieSys::JMP::execute(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaData = eaMode->getData(eaReg, 4);
    cpu->setPc(eaData->getDataAsLong());
    return 8 + eaData->getCycles();
}

std::string GenieSys::JMP::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    stream << "JMP " << eaMode->disassemble(eaReg, 4);
    return stream.str();
}
