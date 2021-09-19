//
// Created by pault on 9/19/2021.
//

#include <GenieSys/CpuOperations/MOVE.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <sstream>

MOVE::MOVE(M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> MOVE::getOpcodes() {
    return getPossibleOpcodes((uint16_t) 0b0000000000000000, std::vector<BitMask<uint16_t>*> {
       &sizeMask,
       &destEaModeMask,
       &destEaRegMask,
       &srcEaModeMask,
       &srcEaRegMask
    });
}

uint8_t MOVE::getSpecificity() {
    return sizeMask.getWidth() + destEaModeMask.getWidth() + destEaRegMask.getWidth() + srcEaRegMask.getWidth() + srcEaModeMask.getWidth();
}

uint8_t MOVE::execute(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t sizeBytes;
    switch (size) {
        case 1:
            sizeBytes = 1;
            break;
        case 2:
            sizeBytes = 4;
            break;
        case 3:
            sizeBytes = 2;
            break;
        default:
            abort();
    }
    uint8_t destModeId = destEaModeMask.apply(opWord);
    auto destMode = cpu->getAddressingMode(destModeId);
    uint8_t destReg = destEaRegMask.apply(opWord);
    uint8_t srcModeId = srcEaModeMask.apply(opWord);
    auto srcMode = cpu->getAddressingMode(srcModeId);
    uint8_t srcReg = srcEaRegMask.apply(opWord);
    auto srcResult = srcMode->getData(srcReg, sizeBytes);
    auto destResult = destMode->getData(destReg, sizeBytes);
    const std::vector<uint8_t> data = srcResult->getData();
    uint8_t oldExtendCcr = cpu->getCcrFlags() & CCR_EXTEND;
    uint8_t negativeFlag = (int8_t)data[0] < 0 ? CCR_NEGATIVE : 0;
    uint8_t zeroFlag = CCR_ZERO;
    for(uint8_t byte : data) {
        if (byte != 0) {
            zeroFlag = 0;
        }
    }
    destResult->write(data);
    cpu->setCcrFlags(oldExtendCcr | negativeFlag | zeroFlag);
    return 4 + destResult->getCycles() + srcResult->getCycles();
}

std::string MOVE::disassemble(uint16_t opWord) {
    uint8_t size = sizeMask.apply(opWord);
    uint8_t sizeBytes;
    std::string sizeStr;
    switch (size) {
        case 1:
            sizeBytes = 1;
            sizeStr = ".b";
            break;
        case 2:
            sizeBytes = 4;
            sizeStr = ".l";
            break;
        case 3:
            sizeBytes = 2;
            sizeStr = ".w";
            break;
        default:
            abort();
    }
    uint8_t destModeId = destEaModeMask.apply(opWord);
    auto destMode = cpu->getAddressingMode(destModeId);
    uint8_t destReg = destEaRegMask.apply(opWord);
    uint8_t srcModeId = srcEaModeMask.apply(opWord);
    auto srcMode = cpu->getAddressingMode(srcModeId);
    uint8_t srcReg = srcEaRegMask.apply(opWord);
    std::stringstream stream;
    stream << "MOVE" << sizeStr << " " << srcMode->disassemble(srcReg, sizeBytes) << "," << destMode->disassemble(destReg, sizeBytes);
    return stream.str();
}
