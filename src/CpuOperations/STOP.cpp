//
// Created by pault on 2/6/2022.
//

#include <GenieSys/CpuOperations/STOP.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <vector>
#include <sstream>

GenieSys::STOP::STOP(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::STOP::getOpcodes() {
    return std::vector<uint16_t>{opWord};
}

uint8_t GenieSys::STOP::getSpecificity() {
    return 0;
}

uint8_t GenieSys::STOP::execute(uint16_t opWord) {
    if (!cpu->isSupervisor()) {
        cpu->trap(GenieSys::TV_PRIVILEGE);
    }
    auto traceEnabled = cpu->isTraceEnabled();
    auto immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    auto immData = immMode->getData(ImmediateDataMode::MODE_ID, 2);
    cpu->setSR(immData->getDataAsWord());
    if (traceEnabled == GenieSys::TRACE_CHANGE) {
        cpu->trap(GenieSys::TV_TRACE);
    }
    else {
        bus->stop();
    }
    return 4;
}

std::string GenieSys::STOP::disassemble(uint16_t opWord) {
    auto immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    std::stringstream stream;
    stream << "STOP " << immMode->disassemble(ImmediateDataMode::MODE_ID, 2);
    return stream.str();
}
