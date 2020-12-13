//
// Created by paul.trampert on 12/6/2020.
//

#include <GenieSys/CpuOperations/Abcd.h>
#include <GenieSys/BitMask.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/getPossibleOpcodes.h>

const uint16_t OPCODE_BASE = 0b1100000100000000;
static BitMask<uint16_t> RX_MASK = BitMask<uint16_t>(11, 3);
static BitMask<uint16_t> RY_MASK = BitMask<uint16_t>(2, 3);
static BitMask<uint16_t> RM_MASK = BitMask<uint16_t>(3, 1);
static BitMask<uint8_t> ONES_DIGIT = BitMask<uint8_t>(3, 4);
static BitMask<uint8_t> TENS_DIGIT = BitMask<uint8_t>(7, 4);

void Abcd::execute(uint16_t opWord) {
    uint16_t destReg = RX_MASK.apply(opWord);
    uint16_t srcReg = RY_MASK.apply(opWord);
    uint16_t rm = RM_MASK.apply(opWord);
    AddressingMode* mode;
    if (rm > 0) {
        mode = cpu->getAddressingMode(AddressRegisterIndirectPreDecrementMode::MODE_ID);
    }
    else {
        mode = cpu->getAddressingMode(DataRegisterDirectMode::MODE_ID);
    }
    auto dest = mode->getData(destReg, 1);
    auto src = mode->getData(srcReg, 1);
    auto destOnesDigit = ONES_DIGIT.apply(dest->getDataAsByte());
    auto destTensDigit = TENS_DIGIT.apply(dest->getDataAsByte());
    auto srcOnesDigit = ONES_DIGIT.apply(src->getDataAsByte());
    auto srcTensDigit = TENS_DIGIT.apply(src->getDataAsByte());
    auto x = (cpu->getCcrFlags() & CCR_EXTEND) >> 4;
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
        newCcrFlags |= CCR_EXTEND | CCR_CARRY;
    }
    if (destTensDigit == 0 && destOnesDigit == 0) {
        newCcrFlags |= CCR_ZERO;
    }
    cpu->setCcrFlags(newCcrFlags);
    dest->write((uint8_t)((destTensDigit << 4) | destOnesDigit));
}

Abcd::Abcd(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {
}

std::vector<uint16_t> Abcd::getOpcodes() {
    return getPossibleOpcodes(OPCODE_BASE, std::vector<BitMask<uint16_t>*>{
        &RX_MASK,
        &RY_MASK,
        &RM_MASK
    });
}
