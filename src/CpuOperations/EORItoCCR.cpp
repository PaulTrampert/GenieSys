//
// Created by paul.trampert on 12/16/2020.
//

#include <GenieSys/CpuOperations/EORItoCCR.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <sstream>


EORItoCCR::EORItoCCR(GenieSys::M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t EORItoCCR::getSpecificity() {
    return 0;
}

std::vector<uint16_t> EORItoCCR::getOpcodes() {
    return std::vector<uint16_t>{BASE_OPCODE};
}

uint8_t EORItoCCR::execute(uint16_t opWord) {
    ImmediateDataMode mode(cpu, bus);
    auto addressingResult = mode.getData(0, 1);
    auto immediate = addressingResult->getDataAsByte();
    auto ccr = cpu->getCcrFlags();
    cpu->setCcrFlags(ccr ^ immediate);
    return 20;
}

std::string EORItoCCR::disassemble(uint16_t opWord) {
    ImmediateDataMode mode(cpu, bus);
    std::stringstream stream;
    stream << "EORI " << mode.disassemble(0, 1) << ",CCR";
    return stream.str();
}
