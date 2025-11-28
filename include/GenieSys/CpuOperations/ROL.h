//
// Created by copilot on implementation of ROL instruction
//

#pragma once
#include "CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class ROL : public CpuOperation {
    private:
        // Register rotate: 1110 count 1 size i/r 11 register
        // Memory rotate:   1110 011 1 11 ea_mode ea_reg
        BitMask<uint16_t> countRegMask = BitMask<uint16_t>(11, 3);
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(7, 2, 0, 2);
        BitMask<uint16_t> irMask = BitMask<uint16_t>(5, 1);  // 0=immediate, 1=register
        BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    public:
        ROL(M68kCpu* cpu, Bus* bus);
        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;

    private:
        bool isMemoryRotate(uint16_t opWord);
        uint8_t executeRegister(uint16_t opWord);
        uint8_t executeMemory(uint16_t opWord);
    };
}
