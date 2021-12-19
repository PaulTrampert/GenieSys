//
// Created by pault on 12/19/2021.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class MOVE_USP : public CpuOperation {
    private:
        uint16_t baseOpWord = 0b0100111001100000;
        BitMask<uint16_t> dirMask = BitMask<uint16_t>(3, 1);
        BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);

    public:
        MOVE_USP(M68kCpu * cpu, Bus * bus);

        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
