//
// Created by pault on 9/19/2021.
//

#include <GenieSys/CpuOperations/MOVE.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <sstream>



static uint8_t byteWordCycleTable[12][9] = {
        {4, 4, 8, 8, 8, 12, 14, 12, 16},
        {4, 4, 8, 8, 8, 12, 14, 12, 16},
        {8, 8, 12, 12, 12, 16, 18, 16, 20},
        {8, 8, 12, 12, 12, 16, 18, 16, 20},
        {10, 10, 14, 14, 14, 18, 20, 18, 22},
        {12, 12, 16, 16, 16, 20, 22, 20, 24},
        {14, 14, 18, 18, 18, 22, 24, 22, 26},
        {12, 12, 16, 16, 16, 20, 22, 20, 24},
        {16, 16, 20, 20, 20, 24, 26, 24, 28},
        {12, 12, 16, 16, 16, 20, 22, 20, 24},
        {14, 14, 18, 18, 18, 22, 24, 22, 26},
        {8, 8, 12, 12, 12, 16, 18, 16, 20}
};

static uint8_t longCycleTable[12][9] = {
        {4, 4, 12, 12, 12, 16, 18, 16, 20},
        {4, 4, 12, 12, 12, 16, 18, 16, 20},
        {12, 12, 20, 20, 20, 24, 26, 24, 28},
        {12, 12, 20, 20, 20, 24, 26, 24, 28},
        {14, 14, 22, 22, 22, 26, 28, 26, 30},
        {16, 16, 24, 24, 24, 28, 30, 28, 32},
        {18, 18, 26, 26, 26, 30, 32, 30, 34},
        {16, 16, 24, 24, 24, 28, 30, 28, 32},
        {20, 20, 28, 28, 28, 32, 34, 32, 36},
        {16, 16, 24, 24, 24, 28, 30, 28, 32},
        {18, 18, 26, 26, 26, 30, 32, 30, 34},
        {12, 12, 20, 20, 20, 24, 26, 24, 28},
};

MOVE::MOVE(GenieSys::M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}

std::vector<uint16_t> MOVE::getOpcodes() {
    return GenieSys::getPossibleOpcodes((uint16_t) 0b0000000000000000, std::vector<GenieSys::BitMask<uint16_t>*> {
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
        case 0b01:
            sizeBytes = 1;
            break;
        case 0b10:
            sizeBytes = 4;
            break;
        case 0b11:
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
    return sizeBytes == 4
        ? longCycleTable[srcResult->getMoveCycleKey()][destResult->getMoveCycleKey()]
        : byteWordCycleTable[srcResult->getMoveCycleKey()][destResult->getMoveCycleKey()];
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
