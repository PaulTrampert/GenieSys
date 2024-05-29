//
// Created by paul on 5/28/24.
//

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "../BitMask.h"
#include "./CpuOperation.h"

#define BCC_CC 0b0100
#define BCC_CS 0b0101
#define BCC_EQ 0b0111
#define BCC_GE 0b1100
#define BCC_GT 0b1110
#define BCC_HI 0b0010
#define BCC_LE 0b1111
#define BCC_LS 0b0011
#define BCC_LT 0b1101
#define BCC_MI 0b1011
#define BCC_NE 0b0110
#define BCC_PL 0b1010
#define BCC_VC 0b1000
#define BCC_VS 0b1001

namespace GenieSys {
    class Bcc : public CpuOperation {
    private:
        const uint16_t baseOpcode = 0b0110000000000000;
        BitMask<uint16_t> conditionMask = BitMask<uint16_t>(11, 4, 0b0010, 0b1111);
        BitMask<uint16_t> displacementMask = BitMask<uint16_t>(7, 8);

        /**
         * Evaluates the condition code and returns whether or not the condition is met.
         * @param condition The Bcc condition code.
         * @return True if the condition is true, false otherwise.
         */
        bool checkCondition(uint8_t condition);
    public:
        Bcc(M68kCpu* cpu, Bus* bus);
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
