//
// Created by paul on 5/2/25.
//
#include <vector>
#include <string>
#include <sstream>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/CpuOperations/SUBQ.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/getCcrFlags.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>

GenieSys::SUBQ::SUBQ(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::SUBQ::getSpecificity() {
    return dataMask.getWidth() + sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> GenieSys::SUBQ::getOpcodes() {
    return getPossibleOpcodes(BASE_OPCODE, std::vector<BitMask<uint16_t>*>{
        &dataMask,
        &sizeMask,
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::SUBQ::execute(uint16_t opWord) {
    uint8_t data = dataMask.apply(opWord);
    uint8_t size = sizeMask.apply(opWord);
    size = 1 << size;
    uint8_t eaMode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto addressingMode = cpu->getAddressingMode(eaMode);
    auto addressingResult = addressingMode->getData(eaReg, size);
    if (data == 0) {
        data = 8;
    }
    if (size == 1 && eaMode == GenieSys::AddressRegisterDirectMode::MODE_ID) {
        size = 2;
    }
    uint8_t cycles = 4;
    switch (size) {
        case 1:
            subtractByte(addressingResult, data);
            if (eaMode != GenieSys::DataRegisterDirectMode::MODE_ID) {
                cycles = 8;
            }
            break;
        case 2:
            subtractWord(addressingResult, data);
            if (eaMode != GenieSys::DataRegisterDirectMode::MODE_ID) {
                cycles = 8;
            }
            break;
        case 4:
            subtractLong(addressingResult, data);
            if (eaMode != GenieSys::DataRegisterDirectMode::MODE_ID && eaMode != GenieSys::AddressRegisterDirectMode::MODE_ID) {
                cycles = 12;
            }
            else {
                cycles = 8;
            }
            break;
        default:
            return cpu->trap(TV_ILLEGAL_INSTR);
    }
    return cycles + addressingResult->getCycles();
}

std::string GenieSys::SUBQ::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t data = dataMask.apply(opWord);
    uint8_t size = sizeMask.apply(opWord);
    size = pow(2, size);
    uint8_t eaMode = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto addressingMode = cpu->getAddressingMode(eaMode);
    if (data == 0) {
        data = 8;
    }
    stream << "SUBQ" << (size == 1 ? ".b" : size == 2 ? ".w" : ".l") << " #" << std::to_string(data) << ", " << addressingMode->disassemble(eaReg, size);
    return stream.str();
}

void GenieSys::SUBQ::subtractByte(std::unique_ptr<AddressingResult> &eaResult, uint8_t data) {
    uint8_t byteOperand = eaResult->getDataAsByte();
    uint8_t byteResult = byteOperand - data;
    cpu->setCcrFlags(GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(byteResult, byteOperand, data));
    eaResult->write(byteResult);
}

void GenieSys::SUBQ::subtractWord(std::unique_ptr<AddressingResult> &eaResult, uint8_t data) {
    uint16_t wordOperand = eaResult->getDataAsWord();
    uint16_t wordResult = wordOperand - data;
    cpu->setCcrFlags(GenieSys::getSubtractionCcrFlags<uint16_t, int16_t>(wordResult, wordOperand, data));
    eaResult->write(wordResult);
}

void GenieSys::SUBQ::subtractLong(std::unique_ptr<AddressingResult> &eaResult, uint8_t data) {
    uint32_t longOperand = eaResult->getDataAsLong();
    uint32_t longResult = longOperand - data;
    cpu->setCcrFlags(GenieSys::getSubtractionCcrFlags<uint32_t, int32_t>(longResult, longOperand, data));
    eaResult->write(longResult);
}
