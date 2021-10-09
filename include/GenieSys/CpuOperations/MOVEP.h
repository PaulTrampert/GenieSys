//
// Created by pault on 9/14/2021.
//

#pragma once

#include "./CpuOperation.h"
#include "../BitMask.h"



class MOVEP : public CpuOperation {
private:
    enum OpMode {
        WORD_MEM_REG = 0b100u,
        LONG_MEM_REG = 0b101u,
        WORD_REG_MEM = 0b110u,
        LONG_REG_MEM = 0b111u,
    };
    GenieSys::BitMask<uint16_t> OpModeMask = GenieSys::BitMask<uint16_t>(8, 3, WORD_MEM_REG, LONG_REG_MEM);
    GenieSys::BitMask<uint16_t> DnMask = GenieSys::BitMask<uint16_t>(11, 3);
    GenieSys::BitMask<uint16_t> AnMask = GenieSys::BitMask<uint16_t>(2, 3);

    void WordMemToReg(uint32_t anAddr, uint8_t dn);
    void LongMemToReg(uint32_t anAddr, uint8_t dn);
    void WordRegToMem(uint32_t anAddr, uint8_t dn);
    void LongRegToMem(uint32_t anAddr, uint8_t dn);

public:
    MOVEP(GenieSys::M68kCpu* cpu, Bus* bus);
    std::string disassemble(uint16_t opWord) override;
    uint8_t execute(uint16_t opWord) override;
    std::vector<uint16_t> getOpcodes() override;
    uint8_t getSpecificity() override;
};
