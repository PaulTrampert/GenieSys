//
// Created by paul on 5/20/24.
//
#include <GenieSys/CpuOperations/MOVEQ.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/signExtend.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <vector>
#include <cstdint>
#include <sstream>

GenieSys::MOVEQ::MOVEQ(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::MOVEQ::getOpcodes() {
    return GenieSys::getPossibleOpcodes(baseOpWord, std::vector<GenieSys::BitMask<uint16_t>*>{
        &dataMask,
        &destMask
    });
}

uint8_t GenieSys::MOVEQ::getSpecificity() {
    return dataMask.getWidth() + destMask.getWidth();
}

uint8_t GenieSys::MOVEQ::execute(uint16_t opWord) {
    uint8_t dataRegister = destMask.apply(opWord);
    auto data = GenieSys::signExtend<uint32_t>(dataMask.apply(opWord), 8);
    cpu->setDataRegister(dataRegister, data);
    return 4;
}

std::string GenieSys::MOVEQ::disassemble(uint16_t opWord) {
    auto data = dataMask.apply(opWord);
    auto dest = destMask.apply(opWord);
    auto destMode = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
    std::stringstream stream;
    stream << "MOVEQ #" << data << "," << destMode->disassemble(dest, 4);
    return stream.str();
}
