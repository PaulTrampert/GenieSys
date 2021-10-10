//
// Created by paul.trampert on 12/16/2020.
//

#include <GenieSys/CpuOperations/ANDItoCCR.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/M68kCpu.h>
#include <sstream>



GenieSys::ANDItoCCR::ANDItoCCR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {

}

uint8_t GenieSys::ANDItoCCR::getSpecificity() {
    return 0;
}

std::vector<uint16_t> GenieSys::ANDItoCCR::getOpcodes() {
    return std::vector<uint16_t>{0b0000001000111100};
}

uint8_t GenieSys::ANDItoCCR::execute(uint16_t opWord) {
    GenieSys::ImmediateDataMode mode(cpu, bus);
    auto addressingResult = mode.getData(0, 1);
    auto immediate = addressingResult->getDataAsByte();
    auto ccr = cpu->getCcrFlags();
    cpu->setCcrFlags(ccr & immediate);
    return 20;
}

std::string GenieSys::ANDItoCCR::disassemble(uint16_t opWord) {
    GenieSys::ImmediateDataMode mode(cpu, bus);
    std::stringstream stream;
    stream << "ANDI " << mode.disassemble(0, 1) << ",CCR";
    return stream.str();
}
