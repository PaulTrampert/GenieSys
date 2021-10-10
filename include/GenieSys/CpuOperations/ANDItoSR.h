//
// Created by paul.trampert on 12/25/2020.
//

#pragma once
#include "./CpuOperation.h"


namespace GenieSys {
    class ANDItoSR : public GenieSys::CpuOperation {
    public:
        ANDItoSR(M68kCpu* cpu, Bus* bus);
        std::vector<uint16_t> getOpcodes() override;
        uint8_t execute(uint16_t opWord) override;
        uint8_t getSpecificity() override;
        std::string disassemble(uint16_t opWord) override;
    };
}