//
// Created by paul on 12/11/21.
//
#include <GenieSys/CpuOperations/UNLK.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <sstream>
#include "GenieSys/AddressingModes/AddressRegisterDirectMode.h"

GenieSys::UNLK::UNLK(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::UNLK::getSpecificity() {
    return regMask.getWidth();
}

std::vector<uint16_t> GenieSys::UNLK::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, &regMask);
}

uint8_t GenieSys::UNLK::execute(uint16_t opWord) {
    uint8_t reg = regMask.apply(opWord);
    uint32_t regData = cpu->getAddressRegister(reg);
    cpu->setStackPointer(regData);
    regData = cpu->stackPopLong();
    cpu->setAddressRegister(reg, regData);
    return 12;
}

std::string GenieSys::UNLK::disassemble(uint16_t opWord) {
    uint8_t reg = regMask.apply(opWord);
    auto addrRegMode = cpu->getAddressingMode(AddressRegisterDirectMode::MODE_ID);
    std::stringstream stream;
    stream << "UNLK " << addrRegMode->disassemble(reg, 4);
    return stream.str();
}
