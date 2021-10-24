//
// Created by pault on 10/24/2021.
//

#include <GenieSys/CpuOperations/SWAP.h>
#include <vector>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <sstream>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>

GenieSys::SWAP::SWAP(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::SWAP::getSpecificity() {
    return regMask.getWidth();
}

std::vector<uint16_t> GenieSys::SWAP::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, &regMask);
}

uint8_t GenieSys::SWAP::execute(uint16_t opWord) {
    uint8_t reg = regMask.apply(opWord);
    uint32_t data = cpu->getDataRegister(reg);
    uint32_t result = ((data << 16) & 0xFFFF0000) | ((data >> 16) & 0x0000FFFF);
    cpu->setDataRegister(reg, result);
    return 4;
}

std::string GenieSys::SWAP::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t reg = regMask.apply(opWord);
    auto addrMode = cpu->getAddressingMode(DataRegisterDirectMode::MODE_ID);
    stream << "SWAP " << addrMode->disassemble(reg, 4);
    return stream.str();
}
