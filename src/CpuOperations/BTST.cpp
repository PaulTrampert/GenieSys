//
// Created by paul.trampert on 4/4/2021.
//

#include <GenieSys/CpuOperations/BTST.h>
#include <GenieSys/getPossibleOpcodes.h>

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
    return std::string();
}
