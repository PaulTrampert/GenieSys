//
// Created by paul.trampert on 11/27/2020.
//

#pragma once


#include "AddressingMode.h"


namespace GenieSys {
    class ProgramCounterIndirectDisplacementMode : public GenieSys::AddressingMode {
    public:
        static const uint8_t MODE_ID = 0b010u;

        ProgramCounterIndirectDisplacementMode(M68kCpu *cpu, Bus *bus);
        uint32_t getAddress(uint8_t regAddr) override;
        uint8_t getModeId() override;
        std::string disassemble(uint8_t regAddr, uint8_t size) override;
        uint8_t getMoveCycleKey() override;
    };
}


