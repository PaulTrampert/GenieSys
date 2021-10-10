//
// Created by pault on 9/14/2021.
//

#pragma once

#include "./CpuOperation.h"
#include "../BitMask.h"


namespace GenieSys {
    class MOVEP : public GenieSys::CpuOperation {
    private:
        enum OpMode {
            WORD_MEM_REG = 0b100u,
            LONG_MEM_REG = 0b101u,
            WORD_REG_MEM = 0b110u,
            LONG_REG_MEM = 0b111u,
        };
        BitMask<uint16_t> OpModeMask = BitMask<uint16_t>(8, 3, WORD_MEM_REG, LONG_REG_MEM);
        BitMask<uint16_t> DnMask = BitMask<uint16_t>(11, 3);
        BitMask<uint16_t> AnMask = BitMask<uint16_t>(2, 3);

        void WordMemToReg(uint32_t anAddr, uint8_t dn);
        void LongMemToReg(uint32_t anAddr, uint8_t dn);
        void WordRegToMem(uint32_t anAddr, uint8_t dn);
        void LongRegToMem(uint32_t anAddr, uint8_t dn);

    public:
        MOVEP(M68kCpu* cpu, Bus* bus);
        std::string disassemble(uint16_t opWord) override;
        uint8_t execute(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
    };
}
