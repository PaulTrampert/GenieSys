//
// Created by paul.trampert on 12/6/2020.
//
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/CpuOperations/Add.h>
#include <GenieSys/BitMask.h>
#include <cmath>
#include <GenieSys/getPossibleOpcodes.h>
#include <sstream>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/getCcrFlags.h>



const uint16_t OPCODE_BASE = 0b1101000000000000;
static GenieSys::BitMask<uint16_t> REG_MASK = GenieSys::BitMask<uint16_t>(11, 3);
static GenieSys::BitMask<uint16_t> DIRECTION = GenieSys::BitMask<uint16_t>(8, 1);
static GenieSys::BitMask<uint16_t> SIZE = GenieSys::BitMask<uint16_t>(7, 2, 0, 2);
static GenieSys::BitMask<uint16_t> EA_MODE = GenieSys::BitMask<uint16_t>(5, 3);
static GenieSys::BitMask<uint16_t> EA_REG = GenieSys::BitMask<uint16_t>(2, 3);

Add::Add(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t Add::execute(uint16_t opWord) {
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
            return (direction == 1 ? 8 : 4) + addrResult->getCycles();
        case 2:
            addWords(direction, dataRegAddr, addrResult.get());
            return (direction == 1 ? 8 : 4) + addrResult->getCycles();
        case 4:
            addLongs(direction, dataRegAddr, addrResult.get());
            if (direction == 1) {
                return 12 + addrResult->getCycles();
            }
            else if (eaMode == GenieSys::DataRegisterDirectMode::MODE_ID
                || eaMode == GenieSys::AddressRegisterDirectMode::MODE_ID
                || (eaMode == GenieSys::ProgramCounterAddressingMode::MODE_ID && eaRegAddr == GenieSys::ImmediateDataMode::MODE_ID)) {
                return 8 + addrResult->getCycles();
            }
            else {
                return 6 + addrResult->getCycles();
            }
        default:
            return 1;
    }
}

void Add::addBytes(uint8_t direction, uint8_t dataRegAddr, GenieSys::AddressingResult *eaResult) {
    uint8_t regOp = cpu->getDataRegister(dataRegAddr) & 0x000000FF;
    uint8_t eaOp = eaResult->getDataAsByte();
    uint8_t result = direction == 1 ? regOp + eaOp : eaOp + regOp;
    uint8_t ccr = GenieSys::getAdditionCcrFlags<uint8_t, int8_t>(result, regOp, eaOp);
    cpu->setCcrFlags(ccr);
    direction == 1 ? eaResult->write(result) : cpu->setDataRegister(dataRegAddr, result);
}

void Add::addWords(uint8_t direction, uint8_t dataRegAddr, GenieSys::AddressingResult *eaResult) {
    uint16_t regOp = cpu->getDataRegister(dataRegAddr) & 0x0000FFFF;
    uint16_t eaOp = eaResult->getDataAsWord();
    uint16_t result = direction == 1 ? regOp + eaOp : eaOp + regOp;
    uint8_t ccr = GenieSys::getAdditionCcrFlags<uint16_t, int16_t>(result, regOp, eaOp);
    cpu->setCcrFlags(ccr);
    direction == 1 ? eaResult->write(result) : cpu->setDataRegister(dataRegAddr, result);
}

void Add::addLongs(uint8_t direction, uint8_t dataRegAddr, GenieSys::AddressingResult *eaResult) {
    uint32_t regOp = cpu->getDataRegister(dataRegAddr);
    uint32_t eaOp = eaResult->getDataAsWord();
    uint32_t result = direction == 1 ? regOp + eaOp : eaOp + regOp;
    uint8_t ccr = GenieSys::getAdditionCcrFlags<uint32_t, int32_t>(result, regOp, eaOp);
    cpu->setCcrFlags(ccr);
    direction == 1 ? eaResult->write(result) : cpu->setDataRegister(dataRegAddr, result);
}

std::vector<uint16_t> Add::getOpcodes() {
    return GenieSys::getPossibleOpcodes(OPCODE_BASE, std::vector<GenieSys::BitMask<uint16_t>*> {
        &REG_MASK,
        &DIRECTION,
        &SIZE,
        &EA_MODE,
        &EA_REG
    });
}

uint8_t Add::getSpecificity() {
    return REG_MASK.getWidth() + DIRECTION.getWidth() + SIZE.getWidth() + EA_MODE.getWidth() + EA_REG.getWidth();
}

std::string Add::disassemble(uint16_t opWord) {
    const std::string mnemonic = "ADD";
    uint8_t direction = DIRECTION.apply(opWord);
    uint8_t size = pow(2, SIZE.apply(opWord));
    uint8_t eaMode = EA_MODE.apply(opWord);
    uint8_t eaRegAddr = EA_REG.apply(opWord);
    uint8_t dataRegAddr = REG_MASK.apply(opWord);
    auto addrMode = cpu->getAddressingMode(eaMode);
    std::stringstream stream;
    stream << mnemonic;
    switch(size) {
        case 1:
            stream << ".b ";
            break;
        case 2:
            stream << ".w ";
            break;
        case 4:
            stream << ".l ";
            break;
        default:
            break;
    }
    if (direction == 1) {
        stream << "D" << (int)dataRegAddr << "," << addrMode->disassemble(eaRegAddr, size);
    }
    else {
        stream << addrMode->disassemble(eaRegAddr, size) << ",D" << (int)dataRegAddr;
    }
    return stream.str();
}
