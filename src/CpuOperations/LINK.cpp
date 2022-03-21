//
// Created by paul on 12/10/21.
//

#include <GenieSys/CpuOperations/LINK.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/signExtend.h>
#include <vector>
#include <sstream>
#include "GenieSys/AddressingModes/AddressRegisterDirectMode.h"
#include "GenieSys/AddressingModes/ProgramCounterAddressingMode.h"
#include "GenieSys/AddressingModes/ImmediateDataMode.h"

GenieSys::LINK::LINK(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::LINK::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, &regMask);
}

uint8_t GenieSys::LINK::getSpecificity() {
    return regMask.getWidth();
}

uint8_t GenieSys::LINK::execute(uint16_t opWord) {
    uint8_t regAddr = regMask.apply(opWord);
    uint32_t addr = cpu->getAddressRegister(regAddr);
    cpu->stackPushLong(addr);
    cpu->setAddressRegister(regAddr, cpu->getStackPointer());
    uint32_t displacement = signExtend(bus->readWord(cpu->getPc()), 16);
    cpu->incrementPc(2);
    cpu->setStackPointer(cpu->getStackPointer() + displacement);
    return 16;
}

std::string GenieSys::LINK::disassemble(uint16_t opWord) {
    uint8_t regAddr = regMask.apply(opWord);
    auto addrMode = cpu->getAddressingMode(AddressRegisterDirectMode::MODE_ID);
    auto immMode = cpu->getAddressingMode(ProgramCounterAddressingMode::MODE_ID);
    std::stringstream stream;
    stream << "LINK " << addrMode->disassemble(regAddr, 4) << ", " << immMode->disassemble(ImmediateDataMode::MODE_ID, 2);
    return stream.str();
}
