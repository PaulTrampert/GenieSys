//
// Created by pault on 3/3/2022.
//

#pragma once
#include "./CpuOperation.h"
#include "../BitMask.h"

namespace GenieSys {
    class JSR : public CpuOperation {
    private:
        uint16_t baseOpWord = 0b0100111010000000;
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    public:
        JSR(M68kCpu *cpu, Bus *bus);

        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}
