//
// Created by pault on 5/3/2022.
//

#pragma once

#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class MOVEM : public CpuOperation {
    private:
        uint16_t baseOpWord = 0b0100100010000000;
        BitMask<uint16_t> dirMask = BitMask<uint16_t>(10, 1);
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(6, 1);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

        /**
         * Total execution time for a MOVEM, per direction and addressing mode.
         *
         * @param dir       DIR_MEM_TO_REG or DIR_REG_TO_MEM.
         * @param size      Transfer size in bytes, 2 or 4.
         * @param eaModeId  Effective address mode.
         * @param eaReg     Effective address register field, the sub mode selector in mode 0b111.
         * @param regCount  Number of registers named by the register list word.
         */
        static uint8_t getMovemCycles(uint8_t dir, uint8_t size, uint8_t eaModeId, uint8_t eaReg, uint8_t regCount);

    public:
        MOVEM(M68kCpu *cpu, Bus *bus);

        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}