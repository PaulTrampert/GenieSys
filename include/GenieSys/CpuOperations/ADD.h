//
// Created by paul.trampert on 12/6/2020.
//

#pragma once
#include "./CpuOperation.h"


namespace GenieSys {
    class AddressingResult;

    class ADD : public GenieSys::CpuOperation {
    private:
        void addBytes(uint8_t direction, uint8_t dataRegAddr, AddressingResult* eaResult);
        void addWords(uint8_t direction, uint8_t dataRegAddr, AddressingResult* eaResult);
        void addLongs(uint8_t direction, uint8_t dataRegAddr, AddressingResult* eaResult);
    public:
        ADD(M68kCpu* cpu, Bus* bus);
        uint8_t execute(uint16_t opWord) override;
        std::vector<uint16_t> getOpcodes() override;
        uint8_t getSpecificity() override;
        std::string disassemble(uint16_t opWord) override;
    };
}