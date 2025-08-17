//
// Created by paul on 7/22/25.
//
#include <cmath>
#include <sstream>
#include <GenieSys/getCcrFlags.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/CpuOperations/ADDQ.h>

GenieSys::ADDQ::ADDQ(M68kCpu* cpu, Bus* bus) : GenieSys::CpuOperation(cpu, bus) {
}

std::string GenieSys::ADDQ::disassemble(uint16_t opWord) {
    std::stringstream result;
    const uint8_t size = 1 << sizeMask.apply(opWord);
    const uint8_t eaModeCode = eaModeMask.apply(opWord);
    const uint8_t eaReg = eaRegMask.apply(opWord);
    uint8_t data = dataMask.apply(opWord);

    if (data == 0)
    {
        data = 8;
    }

    const auto addressingMode = cpu->getAddressingMode(eaModeCode);

    std::string sizeStr;
    switch (size) {
        case 1: sizeStr = ".b"; break;
        case 2: sizeStr = ".w"; break;
        case 4: sizeStr = ".l"; break;
        default: sizeStr = ""; break;
    }

    result << "ADDQ" << sizeStr << " #" << std::to_string(data) << ", " << addressingMode->disassemble(eaReg, size);

    return result.str();
}

uint8_t GenieSys::ADDQ::getSpecificity()
{
    return dataMask.getWidth() + sizeMask.getWidth() + eaModeMask.getWidth() + eaRegMask.getWidth();
}

uint8_t GenieSys::ADDQ::execute(uint16_t opWord)
{
    uint8_t data = dataMask.apply(opWord);
    uint8_t size = 1 << sizeMask.apply(opWord);
    const uint8_t eaMode = eaModeMask.apply(opWord);
    const uint8_t eaReg = eaRegMask.apply(opWord);

    if (size < 4 && eaMode == GenieSys::AddressRegisterDirectMode::MODE_ID) {
        size = 4;
    }

    if (data == 0) {
        data = 8;
    }

    const bool setCcr = eaMode != AddressRegisterDirectMode::MODE_ID;

    const auto addressingMode = cpu->getAddressingMode(eaMode);
    const auto addressingResult = addressingMode->getData(eaReg, size);

    uint8_t byteOperand = 0;
    uint8_t byteResult = 0;
    uint16_t wordOperand = 0;
    uint16_t wordResult = 0;
    uint32_t longOperand = 0;
    uint32_t longResult = 0;

    uint8_t cycles = 8; // Base cycles for ADDQ
    switch (size) {
        case 1:
            byteOperand = addressingResult->getDataAsByte();
            byteResult = static_cast<uint8_t>(byteOperand + data);
            addressingResult->write(byteResult);
            if (setCcr)
            {
                cpu->setCcrFlags(getAdditionCcrFlags<uint8_t, int8_t>(byteResult, byteOperand, data));
            }
            break;
        case 2:
            wordOperand = addressingResult->getDataAsWord();
            wordResult = static_cast<uint16_t>(wordOperand + data);
            addressingResult->write(wordResult);
            if (setCcr)
            {
                cpu->setCcrFlags(getAdditionCcrFlags<uint16_t, int16_t>(wordResult, wordOperand, data));
            }
            break;
        case 4:
            longOperand = addressingResult->getDataAsLong();
            longResult = static_cast<uint32_t>(longOperand + data);
            addressingResult->write(longResult);
            if (setCcr)
            {
                cpu->setCcrFlags(getAdditionCcrFlags<uint32_t, int32_t>(longResult, longOperand, data));
            }
            break;
        default:
            return cpu->trap(TV_ILLEGAL_INSTR); // Invalid size
    }

    if (eaMode == DataRegisterDirectMode::MODE_ID && size < 4)
    {
        cycles = 4;
    }
    else if (eaMode != DataRegisterDirectMode::MODE_ID && eaMode != AddressRegisterDirectMode::MODE_ID)
    {
        cycles = 12;
    }

    return cycles + addressingResult->getCycles();
}

std::vector<uint16_t> GenieSys::ADDQ::getOpcodes()
{
    return getPossibleOpcodes(BASE_OPCODE, std::vector{
        &dataMask,
        &sizeMask,
        &eaModeMask,
        &eaRegMask
    });
}
