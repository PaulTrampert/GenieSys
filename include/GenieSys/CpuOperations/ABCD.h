//
// Created by paul.trampert on 12/6/2020.
//

#pragma once
#include "./CpuOperation.h"


namespace GenieSys {
    /**
     * Add Decimal With Extend (M68KPM.pdf, page 107)
     */
    class ABCD : public GenieSys::CpuOperation {

    public:
        ABCD(M68kCpu* cpu, Bus* bus);
        uint8_t execute(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        std::string disassemble(uint16_t opWord) override;
    };
}