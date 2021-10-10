//
// Created by pault on 9/12/2021.
//

#include <GenieSys/CpuOperations/BSET.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/M68kCpu.h>
#include <sstream>



GenieSys::BSET::BSET(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::BSET::getOpcodes() {
    auto result = GenieSys::getPossibleOpcodes((uint16_t)0b0000000110000000, std::vector<GenieSys::BitMask<uint16_t>*> {
       &DnMask,
       &EaModeMask,
       &EaAddrMask
    });

    auto immResult = GenieSys::getPossibleOpcodes((uint16_t)0b0000100010000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &EaModeMask,
        &EaAddrMask
    });

    for(auto imm : immResult) {
        result.push_back(imm);
    }
    return result;
}

uint8_t GenieSys::BSET::getSpecificity() {
    return DnMask.getWidth() + ImmMask.getWidth() + EaModeMask.getWidth() + EaAddrMask.getWidth();
}

uint8_t GenieSys::BSET::execute(uint16_t opWord) {
    uint8_t cycles;
    bool imm = !ImmMask.apply(opWord);
    uint16_t eaModeId = EaModeMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint16_t eaAddr = EaAddrMask.apply(opWord);
    uint8_t size = eaModeId == GenieSys::DataRegisterDirectMode::MODE_ID ? 4 : 1;
    uint8_t sizeBits = size * 8;
    uint32_t bitNum;
    if (imm) {
        auto immMode = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
        auto immResult = immMode->getData(GenieSys::ImmediateDataMode::MODE_ID, 1);
        bitNum = immResult->getDataAsByte();
        cycles = 12;
    }
    else {
        auto dnMode = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
        auto dnResult = dnMode->getData(DnMask.apply(opWord), 4);
        bitNum = dnResult->getDataAsLong();
        cycles = 8;
    }
    bitNum %= sizeBits;
    auto eaResult = eaMode->getData(eaAddr, size);
    uint32_t eaData = size == 4 ? eaResult->getDataAsLong() : eaResult->getDataAsByte();
    GenieSys::BitMask<uint32_t> mask(bitNum, 1);
    uint8_t testResult = mask.apply(eaData) == 0 ? GenieSys::CCR_ZERO : 0;
    cpu->setCcrFlags((cpu->getCcrFlags() & ~testResult) | testResult);
    eaData = mask.compose(eaData, 1);
    if (size == 4) {
        eaResult->write((uint32_t)eaData);
    }
    else {
        eaResult->write((uint8_t)(eaData & 0x000000FF));
    }
    return cycles + eaResult->getCycles();
}

std::string GenieSys::BSET::disassemble(uint16_t opWord) {
    bool immMode = !ImmMask.apply(opWord);
    uint8_t eaModeId = EaModeMask.apply(opWord);
    uint8_t eaAddr = EaAddrMask.apply(opWord);
    uint8_t destSize = eaModeId == GenieSys::DataRegisterDirectMode::MODE_ID ? 4 : 1;
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::stringstream stream;
    if (immMode) {
        auto imm = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
        auto destData = eaMode->disassemble(eaAddr, destSize);
        stream << "BSET " << imm->disassemble(GenieSys::ImmediateDataMode::MODE_ID, 1) << "," << destData;
    }
    else {
        auto dn = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
        stream << "BSET " << dn->disassemble(DnMask.apply(opWord), 4) << "," << eaMode->disassemble(eaAddr, destSize);
    }
    return stream.str();
}
