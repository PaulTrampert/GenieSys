//
// Created by paul.trampert on 12/6/2020.
//
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/CpuOperations/Add.h>
#include <GenieSys/BitMask.h>
#include <cmath>

static BitMask<uint16_t> REG_MASK = BitMask<uint16_t>(11, 3);
static BitMask<uint16_t> DIRECTION = BitMask<uint16_t>(8, 1);
static BitMask<uint16_t> SIZE = BitMask<uint16_t>(7, 2);
static BitMask<uint16_t> EA_MODE = BitMask<uint16_t>(5, 3);
static BitMask<uint16_t> EA_REG = BitMask<uint16_t>(2, 3);

template<class U, class S>
static uint8_t getCcrFlags(U result, U op1, U op2) {
    uint8_t ccr = 0;
    if (result < op1) {
        ccr |= CCR_OVERFLOW;
    }
    if (((S) result < 0 && (S)op1 > 0 && (S)op2 > 0) ||
        ((S) result > 0 && (S)op1 < 0 && (S)op2 < 0)
            ) {
        ccr |= CCR_EXTEND | CCR_CARRY;
    }
    if (result == 0) {
        ccr |= CCR_ZERO;
    }
    if ((S)result < 0) {
        ccr |= CCR_NEGATIVE;
    }
    return ccr;
}

Add::Add(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

void Add::execute(uint16_t opWord) {
    uint8_t direction = DIRECTION.apply(opWord);
    uint8_t size = pow(2, SIZE.apply(opWord));
    uint8_t eaMode = EA_MODE.apply(opWord);
    uint8_t eaRegAddr = EA_REG.apply(opWord);
    uint8_t dataRegAddr = REG_MASK.apply(opWord);
    auto addrMode = cpu->getAddressingMode(eaMode);
    auto addrResult = addrMode->getData(eaRegAddr, size);
    switch(size) {
        case 1:
            addBytes(direction, dataRegAddr, addrResult.get());
            break;
        case 2:
            addWords(direction, dataRegAddr, addrResult.get());
            break;
        case 4:
            addLongs(direction, dataRegAddr, addrResult.get());
            break;
        default:
            addLongs(direction, dataRegAddr, addrResult.get());
    }
}

void Add::addBytes(uint8_t direction, uint8_t dataRegAddr, AddressingResult *eaResult) {
    uint8_t regOp = cpu->getDataRegister(dataRegAddr) & 0x000000FF;
    uint8_t eaOp = eaResult->getDataAsByte();
    uint8_t result = direction == 1 ? regOp + eaOp : eaOp + regOp;
    uint8_t ccr = getCcrFlags<uint8_t, int8_t>(result, regOp, eaOp);
    cpu->setCcrFlags(ccr);
    direction == 1 ? eaResult->write(result) : cpu->setDataRegister(dataRegAddr, result);
}

void Add::addWords(uint8_t direction, uint8_t dataRegAddr, AddressingResult *eaResult) {
    uint16_t regOp = cpu->getDataRegister(dataRegAddr) & 0x0000FFFF;
    uint16_t eaOp = eaResult->getDataAsWord();
    uint16_t result = direction == 1 ? regOp + eaOp : eaOp + regOp;
    uint8_t ccr = getCcrFlags<uint16_t, int16_t>(result, regOp, eaOp);
    cpu->setCcrFlags(ccr);
    direction == 1 ? eaResult->write(result) : cpu->setDataRegister(dataRegAddr, result);
}

void Add::addLongs(uint8_t direction, uint8_t dataRegAddr, AddressingResult *eaResult) {
    uint32_t regOp = cpu->getDataRegister(dataRegAddr);
    uint32_t eaOp = eaResult->getDataAsWord();
    uint32_t result = direction == 1 ? regOp + eaOp : eaOp + regOp;
    uint8_t ccr = getCcrFlags<uint32_t, int32_t>(result, regOp, eaOp);
    cpu->setCcrFlags(ccr);
    direction == 1 ? eaResult->write(result) : cpu->setDataRegister(dataRegAddr, result);
}
