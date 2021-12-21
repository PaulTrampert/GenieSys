//
// Created by pault on 12/19/2021.
//

#include <GenieSys/CpuOperations/MOVE_USP.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <vector>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/enums.h>
#include <sstream>

GenieSys::MOVE_USP::MOVE_USP(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::MOVE_USP::getSpecificity() {
    return dirMask.getWidth() + regMask.getWidth();
}

std::vector<uint16_t> GenieSys::MOVE_USP::getOpcodes() {
    return getPossibleOpcodes(baseOpWord, std::vector<BitMask<uint16_t>*>{
        &dirMask,
        &regMask
    });
}

uint8_t GenieSys::MOVE_USP::execute(uint16_t opWord) {
    uint8_t dir = dirMask.apply(opWord);
    uint8_t reg = regMask.apply(opWord);
    auto addrRegMode = cpu->getAddressingMode(AddressRegisterDirectMode::MODE_ID);
    auto addrResult = addrRegMode->getData(reg, 4);
    if (dir == 1) {
        addrResult->write(cpu->getStackPointer());
    }
    else if (dir == 0){
        cpu->setStackPointer(addrResult->getDataAsLong());
    }
    else {
        return cpu->trap(GenieSys::TRAP_VECTORS::TV_ILLEGAL_INSTR);
    }
    return 4;
}

std::string GenieSys::MOVE_USP::disassemble(uint16_t opWord) {
    uint8_t dir = dirMask.apply(opWord);
    uint8_t reg = regMask.apply(opWord);
    std::stringstream stream;
    auto addrMode = cpu->getAddressingMode(AddressRegisterDirectMode::MODE_ID);
    stream << "MOVE ";
    if (dir == 1) {
        stream << "USP, " << addrMode->disassemble(reg, 4);
    }
    else {
        stream << addrMode->disassemble(reg, 4) << ", USP";
    }
    return stream.str();
}

