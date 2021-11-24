//
// Created by paul.trampert on 12/25/2020.
//

#include <GenieSys/CpuOperations/EORItoSR.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <sstream>
#include <GenieSys/M68kCpu.h>


GenieSys::EORItoSR::EORItoSR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::EORItoSR::getOpcodes() {
    return std::vector<uint16_t> {0b0000101001111100};
}

uint8_t GenieSys::EORItoSR::getSpecificity() {
    return 0;
}

uint8_t GenieSys::EORItoSR::execute(uint16_t opWord) {
    GenieSys::AddressingMode *mode = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
    auto imm = mode->getData(GenieSys::ImmediateDataMode::MODE_ID, 2);
    if (!cpu->isSupervisor()) {
        return cpu->trap(GenieSys::TV_PRIVILEGE);
    } else {
        cpu->setSR(cpu->getSR() ^ imm->getDataAsWord());
    }
    return 20;
}

std::string GenieSys::EORItoSR::disassemble(uint16_t opWord) {
    GenieSys::AddressingMode* mode = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
    std::stringstream stream;
    stream << "EORI " << mode->disassemble(GenieSys::ImmediateDataMode::MODE_ID, 2) << ",SR";
    return stream.str();
}
