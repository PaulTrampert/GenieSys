//
// Created by copilot on implementation of ROR instruction
//

#include <GenieSys/CpuOperations/ROR.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/M68kCpu.h>
#include <vector>
#include <sstream>
#include <cmath>

GenieSys::ROR::ROR(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {
}

uint8_t GenieSys::ROR::getSpecificity() {
    return countRegMask.getWidth() + sizeMask.getWidth() + irMask.getWidth() + regMask.getWidth();
}

std::vector<uint16_t> GenieSys::ROR::getOpcodes() {
    std::vector<uint16_t> result;
    
    // Register rotate with immediate count: 1110 count 0 size 0 11 register
    // Base: 0b1110 000 0 00 0 11 000 = 0xE018
    std::vector<uint16_t> immByte = getPossibleOpcodes((uint16_t)0xE018, std::vector<BitMask<uint16_t>*>{
        &countRegMask, &regMask
    });
    result.insert(result.end(), immByte.begin(), immByte.end());
    
    // Word size immediate: 0xE058
    std::vector<uint16_t> immWord = getPossibleOpcodes((uint16_t)0xE058, std::vector<BitMask<uint16_t>*>{
        &countRegMask, &regMask
    });
    result.insert(result.end(), immWord.begin(), immWord.end());
    
    // Long size immediate: 0xE098
    std::vector<uint16_t> immLong = getPossibleOpcodes((uint16_t)0xE098, std::vector<BitMask<uint16_t>*>{
        &countRegMask, &regMask
    });
    result.insert(result.end(), immLong.begin(), immLong.end());
    
    // Register rotate with register count: 1110 reg 0 size 1 11 register
    // Base: 0b1110 000 0 00 1 11 000 = 0xE038
    std::vector<uint16_t> regByte = getPossibleOpcodes((uint16_t)0xE038, std::vector<BitMask<uint16_t>*>{
        &countRegMask, &regMask
    });
    result.insert(result.end(), regByte.begin(), regByte.end());
    
    // Word size register: 0xE078
    std::vector<uint16_t> regWord = getPossibleOpcodes((uint16_t)0xE078, std::vector<BitMask<uint16_t>*>{
        &countRegMask, &regMask
    });
    result.insert(result.end(), regWord.begin(), regWord.end());
    
    // Long size register: 0xE0B8
    std::vector<uint16_t> regLong = getPossibleOpcodes((uint16_t)0xE0B8, std::vector<BitMask<uint16_t>*>{
        &countRegMask, &regMask
    });
    result.insert(result.end(), regLong.begin(), regLong.end());
    
    // Memory rotate: 1110 011 0 11 ea_mode ea_reg
    // Base: 0b1110 011 0 11 000 000 = 0xE6C0
    std::vector<uint16_t> memOps = getPossibleOpcodes((uint16_t)0xE6C0, std::vector<BitMask<uint16_t>*>{
        &eaModeMask, &eaRegMask
    });
    result.insert(result.end(), memOps.begin(), memOps.end());
    
    return result;
}

bool GenieSys::ROR::isMemoryRotate(uint16_t opWord) {
    // Memory rotate: bits 11-8 = 0b0110 (0x6) and bits 7-6 = 0b11
    return ((opWord >> 8) & 0x0F) == 0x06 && ((opWord >> 6) & 0x03) == 0x03;
}

uint8_t GenieSys::ROR::execute(uint16_t opWord) {
    if (isMemoryRotate(opWord)) {
        return executeMemory(opWord);
    }
    return executeRegister(opWord);
}

uint8_t GenieSys::ROR::executeRegister(uint16_t opWord) {
    uint8_t countReg = countRegMask.apply(opWord);
    uint8_t size = sizeMask.apply(opWord);
    uint8_t ir = irMask.apply(opWord);
    uint8_t reg = regMask.apply(opWord);
    
    // Get rotate count
    uint8_t count;
    if (ir == 0) {
        // Immediate mode: count of 0 means 8
        count = countReg == 0 ? 8 : countReg;
    } else {
        // Register mode: count is value mod 64
        count = cpu->getDataRegister(countReg) % 64;
    }
    
    uint32_t data = cpu->getDataRegister(reg);
    uint8_t ccr = cpu->getCcrFlags() & CCR_EXTEND;  // X not affected
    bool lastBitOut = false;
    
    switch (size) {
        case 0: {  // Byte
            uint8_t byteData = data & 0xFF;
            if (count > 0) {
                uint8_t rotCount = count % 8;
                if (rotCount == 0) rotCount = 8;
                // For right rotation, the last bit rotated out of LSB goes into MSB
                byteData = (byteData >> rotCount) | (byteData << (8 - rotCount));
                lastBitOut = (byteData >> 7) & 1;  // The bit that was rotated out of LSB is now in MSB
            }
            cpu->setDataRegister(reg, byteData);
            if ((int8_t)byteData < 0) ccr |= CCR_NEGATIVE;
            if (byteData == 0) ccr |= CCR_ZERO;
            if (count > 0 && lastBitOut) ccr |= CCR_CARRY;
            break;
        }
        case 1: {  // Word
            uint16_t wordData = data & 0xFFFF;
            if (count > 0) {
                uint8_t rotCount = count % 16;
                if (rotCount == 0) rotCount = 16;
                // For right rotation, the last bit rotated out of LSB goes into MSB
                wordData = (wordData >> rotCount) | (wordData << (16 - rotCount));
                lastBitOut = (wordData >> 15) & 1;  // The bit that was rotated out of LSB is now in MSB
            }
            cpu->setDataRegister(reg, wordData);
            if ((int16_t)wordData < 0) ccr |= CCR_NEGATIVE;
            if (wordData == 0) ccr |= CCR_ZERO;
            if (count > 0 && lastBitOut) ccr |= CCR_CARRY;
            break;
        }
        case 2: {  // Long
            uint32_t longData = data;
            if (count > 0) {
                uint8_t rotCount = count % 32;
                if (rotCount == 0) rotCount = 32;
                // For right rotation, the last bit rotated out of LSB goes into MSB
                longData = (longData >> rotCount) | (longData << (32 - rotCount));
                lastBitOut = (longData >> 31) & 1;  // The bit that was rotated out of LSB is now in MSB
            }
            cpu->setDataRegister(reg, longData);
            if ((int32_t)longData < 0) ccr |= CCR_NEGATIVE;
            if (longData == 0) ccr |= CCR_ZERO;
            if (count > 0 && lastBitOut) ccr |= CCR_CARRY;
            break;
        }
        default:
            return cpu->trap(TV_ILLEGAL_INSTR);
    }
    
    cpu->setCcrFlags(ccr);
    
    // Timing: 6 + 2n for byte/word, 8 + 2n for long
    return (size == 2) ? (8 + 2 * count) : (6 + 2 * count);
}

uint8_t GenieSys::ROR::executeMemory(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, 2);  // Memory rotate is always word size
    
    uint16_t data = eaResult->getDataAsWord();
    
    // Rotate right by 1
    bool lastBitOut = data & 1;
    data = (data >> 1) | (lastBitOut ? 0x8000 : 0);
    
    eaResult->write(data);
    
    uint8_t ccr = cpu->getCcrFlags() & CCR_EXTEND;  // X not affected
    if ((int16_t)data < 0) ccr |= CCR_NEGATIVE;
    if (data == 0) ccr |= CCR_ZERO;
    if (lastBitOut) ccr |= CCR_CARRY;
    cpu->setCcrFlags(ccr);
    
    return 8 + eaResult->getCycles();
}

std::string GenieSys::ROR::disassemble(uint16_t opWord) {
    std::stringstream stream;
    stream << "ROR";
    
    if (isMemoryRotate(opWord)) {
        uint8_t eaModeId = eaModeMask.apply(opWord);
        uint8_t eaReg = eaRegMask.apply(opWord);
        auto eaMode = cpu->getAddressingMode(eaModeId);
        stream << ".w " << eaMode->disassemble(eaReg, 2);
    } else {
        uint8_t countReg = countRegMask.apply(opWord);
        uint8_t size = sizeMask.apply(opWord);
        uint8_t ir = irMask.apply(opWord);
        uint8_t reg = regMask.apply(opWord);
        
        switch (size) {
            case 0: stream << ".b "; break;
            case 1: stream << ".w "; break;
            case 2: stream << ".l "; break;
            default: stream << " "; break;
        }
        
        auto dataRegMode = cpu->getAddressingMode(DataRegisterDirectMode::MODE_ID);
        
        if (ir == 0) {
            // Immediate
            uint8_t count = countReg == 0 ? 8 : countReg;
            stream << "#" << (int)count << ", " << dataRegMode->disassemble(reg, pow(2, size));
        } else {
            // Register
            stream << dataRegMode->disassemble(countReg, 4) << ", " << dataRegMode->disassemble(reg, pow(2, size));
        }
    }
    
    return stream.str();
}
