//
// Created by paul.trampert on 12/16/2020.
//

#include <GenieSys/CpuOperations/ORItoCCR.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>

ORItoCCR::ORItoCCR(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t ORItoCCR::getSpecificity() {
    return 0;
}

std::vector<uint16_t> ORItoCCR::getOpcodes() {
    return std::vector<uint16_t>{0b0000000000111100};
}

void ORItoCCR::execute(uint16_t opWord) {
    ImmediateDataMode mode(cpu, bus);
    auto addressingResult = mode.getData(0, 1);
    auto immediate = addressingResult->getDataAsByte();
    auto ccr = cpu->getCcrFlags();
    cpu->setCcrFlags(ccr | immediate);
}
