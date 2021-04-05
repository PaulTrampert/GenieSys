//
// Created by paul.trampert on 4/4/2021.
//

#include <GenieSys/CpuOperations/BTST.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <sstream>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>

BTST::BTST(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> BTST::getOpcodes() {
    std::vector<uint16_t> result = getPossibleOpcodes((uint16_t)0b0000000100000000, std::vector<BitMask<uint16_t>*> {
        &DnMask,
        &EaModeMask,
        &EaAddrMask
    });

    std::vector<uint16_t> immResults = getPossibleOpcodes((uint16_t)0b0000100000000000, std::vector<BitMask<uint16_t>*> {
        &EaModeMask,
        &EaAddrMask
    });

    for (auto immResult : immResults) {
        result.push_back(immResult);
    }
    return result;
}

uint8_t BTST::getSpecificity() {
    return DnMask.getWidth() + ImmMask.getWidth() + EaModeMask.getWidth() + EaAddrMask.getWidth();
}

uint8_t BTST::execute(uint16_t opWord) {
    return 0;
}

std::string BTST::disassemble(uint16_t opWord) {
    bool immMode = !ImmMask.apply(opWord);
    uint8_t eaModeId = EaModeMask.apply(opWord);
    uint8_t eaAddr = EaAddrMask.apply(opWord);
    uint8_t destSize = eaModeId == DataRegisterDirectMode::MODE_ID ? 4 : 1;
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::stringstream stream;
    if (immMode) {
        auto imm = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
        auto destData = eaMode->disassemble(eaAddr, destSize);
        stream << "BTST " << imm->disassemble(ImmediateDataMode::MODE_ID, 1) << "," << destData;
    }
    else {
        auto dn = cpu->getAddressingMode(DataRegisterDirectMode::MODE_ID);
        stream << "BTST " << dn->disassemble(DnMask.apply(opWord), 4) << "," << eaMode->disassemble(eaAddr, destSize);
    }
    return stream.str();
}
