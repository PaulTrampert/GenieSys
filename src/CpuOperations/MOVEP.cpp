//
// Created by pault on 9/14/2021.
//
#include <GenieSys/CpuOperations/MOVEP.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <vector>

MOVEP::MOVEP(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::string MOVEP::disassemble(uint16_t opWord) {
    return std::string();
}

uint8_t MOVEP::execute(uint16_t opWord) {
    return 0;
}

std::vector<uint16_t> MOVEP::getOpcodes() {
    return getPossibleOpcodes((uint16_t) 0b0000000000001000, std::vector<BitMask<uint16_t>*> {
        &DnMask,
        &AnMask,
        &OpModeMask
    });
}

uint8_t MOVEP::getSpecificity() {
    return DnMask.getWidth() + AnMask.getWidth() + OpModeMask.getWidth();
}
