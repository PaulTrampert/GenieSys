//
// Created by paul.trampert on 12/16/2020.
//

#include <GenieSys/CpuOperations/ORItoCCR.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <sstream>
#include <GenieSys/M68kCpu.h>


GenieSys::ORItoCCR::ORItoCCR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::ORItoCCR::getSpecificity() {
    return 0;
}

std::vector<uint16_t> GenieSys::ORItoCCR::getOpcodes() {
    return std::vector<uint16_t>{0b0000000000111100};
}

uint8_t GenieSys::ORItoCCR::execute(uint16_t opWord) {
    GenieSys::ImmediateDataMode mode(cpu, bus);
    auto addressingResult = mode.getData(0, 1);
    auto immediate = addressingResult->getDataAsByte();
    auto ccr = cpu->getCcrFlags();
    cpu->setCcrFlags(ccr | immediate);
    return 20;
}

std::string GenieSys::ORItoCCR::disassemble(uint16_t opWord) {
    GenieSys::ImmediateDataMode mode(cpu, bus);
    std::stringstream stream;
    stream << "ORI " << mode.disassemble(0, 1) << ",CCR";
    return stream.str();
}
