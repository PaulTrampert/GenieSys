//
// Created by paul.trampert on 12/16/2020.
//

#include <GenieSys/CpuOperations/EORItoCCR.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <sstream>
#include <GenieSys/M68kCpu.h>


GenieSys::EORItoCCR::EORItoCCR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {

}

uint8_t GenieSys::EORItoCCR::getSpecificity() {
    return 0;
}

std::vector<uint16_t> GenieSys::EORItoCCR::getOpcodes() {
    return std::vector<uint16_t>{BASE_OPCODE};
}

uint8_t GenieSys::EORItoCCR::execute(uint16_t opWord) {
    GenieSys::ImmediateDataMode mode(cpu, bus);
    auto addressingResult = mode.getData(0, 1);
    auto immediate = addressingResult->getDataAsByte();
    auto ccr = cpu->getCcrFlags();
    cpu->setCcrFlags(ccr ^ immediate);
    return 20;
}

std::string GenieSys::EORItoCCR::disassemble(uint16_t opWord) {
    GenieSys::ImmediateDataMode mode(cpu, bus);
    std::stringstream stream;
    stream << "EORI " << mode.disassemble(0, 1) << ",CCR";
    return stream.str();
}
