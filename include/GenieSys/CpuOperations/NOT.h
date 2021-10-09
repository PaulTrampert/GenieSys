//
// Created by pault on 10/4/2021.
//

#pragma once
#include "CpuOperation.h"
#include "../AddressingModes/AddressingMode.h"



/**
 * Documented on page 253 of docs/68KPM.pdf
 */
class NOT : public CpuOperation {
private:
    GenieSys::BitMask<uint16_t> sizeMask = GenieSys::BitMask<uint16_t>(7, 2, 0, 2);
    GenieSys::BitMask<uint16_t> eaModeMask = GenieSys::BitMask<uint16_t>(5, 3);
    GenieSys::BitMask<uint16_t> eaRegMask = GenieSys::BitMask<uint16_t>(2, 3);

    uint8_t notByte(AddressingResult* eaResult, uint8_t eaModeId);
    uint8_t notWord(AddressingResult* eaResult, uint8_t eaModeId);
    uint8_t notLong(AddressingResult* eaResult, uint8_t eaModeId);

    template <class U, class S>
    uint8_t getCcrFlags(U result) {
        uint8_t ccr = cpu->getCcrFlags() & GenieSys::CCR_EXTEND;
        if ((S) result < 0) {
            ccr |= GenieSys::CCR_NEGATIVE;
        }
        if (result == 0) {
            ccr |= GenieSys::CCR_ZERO;
        }
        return ccr;
    }

public:
    NOT(GenieSys::M68kCpu* cpu, GenieSys::Bus* bus);

    uint8_t getSpecificity() override;
    std::vector<uint16_t> getOpcodes() override;
    uint8_t execute(uint16_t opWord) override;
    std::string disassemble(uint16_t opWord) override;
};