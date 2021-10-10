//
// Created by paul.trampert on 4/4/2021.
//

#include <GenieSys/CpuOperations/BTST.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <sstream>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>



GenieSys::BTST::BTST(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::BTST::getOpcodes() {
    std::vector<uint16_t> result = GenieSys::getPossibleOpcodes((uint16_t)0b0000000100000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &DnMask,
        &EaModeMask,
        &EaAddrMask
    });

    std::vector<uint16_t> immResults = GenieSys::getPossibleOpcodes((uint16_t)0b0000100000000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &EaModeMask,
        &EaAddrMask
    });

    for (auto immResult : immResults) {
        result.push_back(immResult);
    }
    return result;
}

uint8_t GenieSys::BTST::getSpecificity() {
    return DnMask.getWidth() + ImmMask.getWidth() + EaModeMask.getWidth() + EaAddrMask.getWidth();
}

uint8_t GenieSys::BTST::execute(uint16_t opWord) {
    uint8_t cycles;
    bool immMode = !ImmMask.apply(opWord);
    uint8_t eaModeId = EaModeMask.apply(opWord);
    uint8_t eaAddr = EaAddrMask.apply(opWord);
    uint8_t destSize = eaModeId == GenieSys::DataRegisterDirectMode::MODE_ID ? 4 : 1;
    uint8_t destSizeBits = destSize * 8;
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint32_t bitNum;
    if (immMode) {
        auto imm = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
        auto immData = imm->getData(GenieSys::ImmediateDataMode::MODE_ID, 1);
        bitNum = immData->getDataAsByte();
        cycles = eaModeId == GenieSys::DataRegisterDirectMode::MODE_ID ? 10 : 8;
    }
    else {
        auto dn = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
        auto dnData = dn->getData(DnMask.apply(opWord), 4);
        bitNum = dnData->getDataAsLong();
        cycles = eaModeId == GenieSys::DataRegisterDirectMode::MODE_ID ? 6 : 4;
    }
    bitNum = bitNum % destSizeBits;
    auto eaData = eaMode->getData(eaAddr, destSize);
    uint32_t eaNum = destSize == 4 ? eaData->getDataAsLong() : eaData->getDataAsByte();
    uint8_t result = ((eaNum >> bitNum) & 1) == 0 ? GenieSys::CCR_ZERO : 0;
    cpu->setCcrFlags((cpu->getCcrFlags() & ~result) | result);
    return cycles + eaData->getCycles();
}

std::string GenieSys::BTST::disassemble(uint16_t opWord) {
    bool immMode = !ImmMask.apply(opWord);
    uint8_t eaModeId = EaModeMask.apply(opWord);
    uint8_t eaAddr = EaAddrMask.apply(opWord);
    uint8_t destSize = eaModeId == GenieSys::DataRegisterDirectMode::MODE_ID ? 4 : 1;
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::stringstream stream;
    if (immMode) {
        auto imm = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
        auto destData = eaMode->disassemble(eaAddr, destSize);
        stream << "BTST " << imm->disassemble(GenieSys::ImmediateDataMode::MODE_ID, 1) << "," << destData;
    }
    else {
        auto dn = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
        stream << "BTST " << dn->disassemble(DnMask.apply(opWord), 4) << "," << eaMode->disassemble(eaAddr, destSize);
    }
    return stream.str();
}
