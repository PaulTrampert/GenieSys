//
// Created by paul.trampert on 12/25/2020.
//

#include <GenieSys/CpuOperations/ANDItoSR.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <sstream>

ANDItoSR::ANDItoSR(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> ANDItoSR::getOpcodes() {
    return std::vector<uint16_t> {0b0000001001111100};
}

uint8_t ANDItoSR::getSpecificity() {
    return 0;
}

uint8_t ANDItoSR::execute(uint16_t opWord) {
    AddressingMode *mode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    auto imm = mode->getData(ImmediateDataMode::MODE_ID, 2);
    if (!cpu->isSupervisor()) {
        cpu->trap(TV_PRIVILEGE);
    } else {
        cpu->setSR(cpu->getSR() & imm->getDataAsWord());
    }
    return 20;
}

std::string ANDItoSR::disassemble(uint16_t opWord) {
    AddressingMode* mode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    std::stringstream stream;
    stream << "ANDI " << mode->disassemble(ImmediateDataMode::MODE_ID, 2) << ",SR";
    return stream.str();
}
