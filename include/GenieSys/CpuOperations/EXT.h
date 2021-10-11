//
// Created by pault on 10/10/2021.
//

#pragma once

#include "CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
#define CPU_INSTR_EXT_BYTE_TO_WORD 0b010u
#define CPU_INSTR_EXT_WORD_TO_LONG 0b011u

    /**
     * Documented on page 211 of 68KPM.pdf
     */
    class EXT : public GenieSys::CpuOperation {
    private:
        BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);
        BitMask<uint16_t> opModeMask = BitMask<uint16_t>(8, 3);

    public:
        EXT(M68kCpu* cpu, Bus* bus);
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}