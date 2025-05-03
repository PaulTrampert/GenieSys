//
// Created by paul on 5/2/25.
//

#pragma once

#include "CpuOperation.h"
#include "../BitMask.h"
#include "GenieSys/AddressingModes/AddressingMode.h"

namespace GenieSys {
    class SUBQ : public GenieSys::CpuOperation {
    private:
        const uint16_t BASE_OPCODE = 0b0101000100000000;
        BitMask<uint16_t> dataMask = BitMask<uint16_t>(11, 3);
        BitMask<uint16_t> sizeMask = BitMask<uint16_t>(7, 2);
        BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
        BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

        void subtractByte(std::unique_ptr<AddressingResult>& eaResult, uint8_t data);
        void subtractWord(std::unique_ptr<AddressingResult>& eaResult, uint8_t data);
        void subtractLong(std::unique_ptr<AddressingResult>& eaResult, uint8_t data);

    public:
        SUBQ(M68kCpu* cpu, Bus* bus);
        std::string disassemble(uint16_t opWord) override;
        uint8_t getSpecificity() override;
        uint8_t execute(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
    };
}
