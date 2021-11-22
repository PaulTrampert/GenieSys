//
// Created by paul on 11/18/21.
//

#include <GenieSys/CpuOperations/TST.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include<sstream>
#include <vector>

GenieSys::TST::TST(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::TST::getSpecificity() {
    return sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> GenieSys::TST::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, std::vector<BitMask<uint16_t>*>{
        &sizeMask,
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::TST::execute(uint16_t opWord) {
    uint8_t size = 1 << sizeMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, size);
    auto ccr = cpu->getCcrFlags() & CCR_EXTEND;
    switch (size) {
        case 1:
            ccr = exec<uint8_t, int8_t>(eaResult->getDataAsByte(), ccr);
            break;
        case 2:
            ccr = exec<uint16_t, int16_t>(eaResult->getDataAsWord(), ccr);
            break;
        case 4:
            ccr = exec<uint32_t, int32_t>(eaResult->getDataAsLong(), ccr);
            break;
        default:
            cpu->trap(TV_ILLEGAL_INSTR);
    }
    cpu->setCcrFlags(ccr);
    return 4 + eaResult->getCycles();
}

std::string GenieSys::TST::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t size = 1 << sizeMask.apply(opWord);
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::string sizeStr;
    switch (size) {
        case 1:
            sizeStr = ".b";
            break;
        case 2:
            sizeStr = ".w";
            break;
        case 4:
            sizeStr = ".l";
            break;
        default:
            sizeStr = "";
    }
    stream << "TST" << sizeStr << " " << eaMode->disassemble(eaReg, size);
    return stream.str();
}
