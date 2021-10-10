//
// Created by pault on 10/2/2021.
//

#pragma once


#include "CpuOperation.h"
#include "../BitMask.h"


namespace GenieSys {
    class AddressingResult;

    class NEGX : public GenieSys::CpuOperation {
    private:
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(7, 2, 0, 2);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

        uint8_t negxByte(std::unique_ptr<AddressingResult> &eaResult, uint8_t oldCcr, uint8_t extendBit);
        uint8_t negxWord(std::unique_ptr<AddressingResult> &eaResult, uint8_t oldCcr, uint8_t extendBit);
        uint8_t negxLong(std::unique_ptr<AddressingResult> &eaResult, uint8_t oldCcr, uint8_t extendBit);

    public:
        NEGX(M68kCpu* cpu, Bus* bus);
        uint8_t getSpecificity() override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        std::string disassemble(uint16_t opWord) override;
    };
}