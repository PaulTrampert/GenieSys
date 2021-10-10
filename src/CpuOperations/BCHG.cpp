//
// Created by pault on 8/26/2021.
//
#include <GenieSys/CpuOperations/BCHG.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/M68kCpu.h>
#include <sstream>



GenieSys::BCHG::BCHG(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {

}

std::vector<uint16_t> GenieSys::BCHG::getOpcodes() {
    auto results = GenieSys::getPossibleOpcodes((uint16_t)0b0000000101000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &DnMask,
        &EaModeMask,
        &EaAddrMask
    });

    auto immModeResults = GenieSys::getPossibleOpcodes((uint16_t)0b0000100001000000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &EaModeMask,
        &EaAddrMask
    });

    for (auto immResult:immModeResults) {
        results.push_back(immResult);
    }
    return results;
}

uint8_t GenieSys::BCHG::getSpecificity() {
    return DnMask.getWidth() + ImmMask.getWidth() + EaModeMask.getWidth() + EaAddrMask.getWidth();
}

uint8_t GenieSys::BCHG::execute(uint16_t opWord) {
    uint8_t cycles;
    bool imm = !ImmMask.apply(opWord);
    uint8_t eaModeId = EaModeMask.apply(opWord);
    uint8_t eaAddr = EaAddrMask.apply(opWord);
    uint8_t destSize = eaModeId == GenieSys::DataRegisterDirectMode::MODE_ID ? 4 : 1;
    uint8_t destSizeBits = destSize * 8;
    auto eaMode = cpu->getAddressingMode(eaModeId);
    uint32_t bitNum;
    if (imm) {
        auto immMode = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
        auto immData = immMode->getData(GenieSys::ImmediateDataMode::MODE_ID, 1);
        bitNum = immData->getDataAsByte();
        cycles = 12;
    }
    else {
        auto dn = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
        auto dnData = dn->getData(DnMask.apply(opWord), 4);
        bitNum = dnData->getDataAsWord();
        cycles = 8;
    }
    bitNum = bitNum % destSizeBits;
    auto eaData = eaMode->getData(eaAddr, destSize);
    uint32_t data = destSize == 4 ? eaData->getDataAsLong() : eaData->getDataAsByte();
    GenieSys::BitMask<uint32_t> mask(bitNum, 1);
    uint32_t bit = mask.apply(data);
    auto result = bit == 0 ? GenieSys::CCR_ZERO : 0;
    cpu->setCcrFlags((cpu->getCcrFlags() & ~result) | result);
    data = mask.compose(data, bit == 1 ? 0 : 1);
    if (destSize == 4) {
        eaData->write((uint32_t) data);
    }
    else {
        eaData->write((uint8_t) (data & 0x000000FF));
    }
    return cycles + eaData->getCycles();
}

std::string GenieSys::BCHG::disassemble(uint16_t opWord) {
    bool immMode = !ImmMask.apply(opWord);
    uint8_t eaModeId = EaModeMask.apply(opWord);
    uint8_t eaAddr = EaAddrMask.apply(opWord);
    uint8_t destSize = eaModeId == GenieSys::DataRegisterDirectMode::MODE_ID ? 4 : 1;
    auto eaMode = cpu->getAddressingMode(eaModeId);
    std::stringstream stream;
    if (immMode) {
        auto imm = cpu->getAddressingMode(GenieSys::ProgramCounterAddressingMode::MODE_ID);
        auto destData = eaMode->disassemble(eaAddr, destSize);
        stream << "BCHG " << imm->disassemble(GenieSys::ImmediateDataMode::MODE_ID, 1) << "," << destData;
    }
    else {
        auto dn = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
        stream << "BCHG " << dn->disassemble(DnMask.apply(opWord), 4) << "," << eaMode->disassemble(eaAddr, destSize);
    }
    return stream.str();
}
