//
// Created by paul.trampert on 12/25/2020.
//

#include <GenieSys/CpuOperations/ORItoSR.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <sstream>

ORItoSR::ORItoSR(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> ORItoSR::getOpcodes() {
    return std::vector<uint16_t> {0b0000000011111100};
}

uint8_t ORItoSR::getSpecificity() {
    return 0;
}

uint8_t ORItoSR::execute(uint16_t opWord) {
    AddressingMode *mode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    auto imm = mode->getData(ImmediateDataMode::MODE_ID, 2);
    if (!cpu->isSupervisor()) {
        cpu->trap(TV_PRIVILEGE);
    } else {
        cpu->setSR(cpu->getSR() | imm->getDataAsWord());
    }
    return 20;
}

std::string ORItoSR::disassemble(uint16_t opWord) {
    AddressingMode* mode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    auto imm = mode->getData(ImmediateDataMode::MODE_ID, 2);
    std::stringstream stream;
    stream << "ORI #" << imm->getDataAsWord() << ",SR";
    return stream.str();
}
