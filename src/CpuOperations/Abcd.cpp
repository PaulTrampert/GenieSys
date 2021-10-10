//
// Created by paul.trampert on 12/6/2020.
//

#include <GenieSys/CpuOperations/Abcd.h>
#include <GenieSys/BitMask.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <sstream>



const uint16_t OPCODE_BASE = 0b1100000100000000;
static GenieSys::BitMask<uint16_t> RX_MASK = GenieSys::BitMask<uint16_t>(11, 3);
static GenieSys::BitMask<uint16_t> RY_MASK = GenieSys::BitMask<uint16_t>(2, 3);
static GenieSys::BitMask<uint16_t> RM_MASK = GenieSys::BitMask<uint16_t>(3, 1);
static GenieSys::BitMask<uint8_t> ONES_DIGIT = GenieSys::BitMask<uint8_t>(3, 4);
static GenieSys::BitMask<uint8_t> TENS_DIGIT = GenieSys::BitMask<uint8_t>(7, 4);
static std::array<uint8_t, 2> cycles = {6,18};

uint8_t GenieSys::Abcd::execute(uint16_t opWord) {
    uint16_t destReg = RX_MASK.apply(opWord);
    uint16_t srcReg = RY_MASK.apply(opWord);
    uint16_t rm = RM_MASK.apply(opWord);
    GenieSys::AddressingMode* mode;
    if (rm > 0) {
        mode = cpu->getAddressingMode(GenieSys::AddressRegisterIndirectPreDecrementMode::MODE_ID);
    }
    else {
        mode = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
    }
    auto dest = mode->getData(destReg, 1);
    auto src = mode->getData(srcReg, 1);
    auto destOnesDigit = ONES_DIGIT.apply(dest->getDataAsByte());
    auto destTensDigit = TENS_DIGIT.apply(dest->getDataAsByte());
    auto srcOnesDigit = ONES_DIGIT.apply(src->getDataAsByte());
    auto srcTensDigit = TENS_DIGIT.apply(src->getDataAsByte());
    auto x = (cpu->getCcrFlags() & GenieSys::CCR_EXTEND) >> 4;
    destOnesDigit = destOnesDigit + srcOnesDigit + x;
    if (destOnesDigit >= 10) {
        x = 1;
        destOnesDigit -= 10;
    }
    else {
        x = 0;
    }
    destTensDigit = destTensDigit + srcTensDigit + x;
    if (destTensDigit >= 10) {
        x = 1;
        destTensDigit -= 10;
    }
    else {
        x = 0;
    }
    uint8_t newCcrFlags = 0;
    if (x == 1) {
        newCcrFlags |= GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY;
    }
    if (destTensDigit == 0 && destOnesDigit == 0) {
        newCcrFlags |= GenieSys::CCR_ZERO;
    }
    cpu->setCcrFlags(newCcrFlags);
    dest->write((uint8_t)((destTensDigit << 4) | destOnesDigit));
    return cycles[rm];
}

GenieSys::Abcd::Abcd(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : GenieSys::CpuOperation(cpu, bus) {
}

std::vector<uint16_t> GenieSys::Abcd::getOpcodes() {
    return GenieSys::getPossibleOpcodes(OPCODE_BASE, std::vector<GenieSys::BitMask<uint16_t>*>{
        &RX_MASK,
        &RY_MASK,
        &RM_MASK
    });
}

uint8_t GenieSys::Abcd::getSpecificity() {
    return RX_MASK.getWidth() + RY_MASK.getWidth() + RM_MASK.getWidth();
}

std::string GenieSys::Abcd::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint16_t destReg = RX_MASK.apply(opWord);
    uint16_t srcReg = RY_MASK.apply(opWord);
    uint16_t rm = RM_MASK.apply(opWord);
    GenieSys::AddressingMode* mode;
    if (rm > 0) {
        mode = cpu->getAddressingMode(GenieSys::AddressRegisterIndirectPreDecrementMode::MODE_ID);
    }
    else {
        mode = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
    }
    stream << "ABCD " << mode->disassemble(srcReg, 1) << "," << mode->disassemble(destReg, 1);
    return stream.str();
}
