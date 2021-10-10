//
// Created by paul.trampert on 11/26/2020.
//

#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "GenieSys/M68kCpu.h"
#include "GenieSys/Bus.h"
#include "GenieSys/BitMask.h"


namespace GenieSys {
    class AddressingResult {
    protected:
        M68kCpu* cpu;
        Bus* bus;
        uint32_t address;
        uint8_t cycles;
        std::vector<uint8_t> data;
        uint8_t moveCycleKey;

    public:
        AddressingResult(M68kCpu *cpu, Bus *bus, uint32_t address, std::vector<uint8_t> data, uint8_t cycles, uint8_t moveCycleKey);
        ~AddressingResult() = default;
        uint8_t getCycles();
        uint8_t getMoveCycleKey();
        std::vector<uint8_t> getData();
        uint8_t getDataAsByte();
        uint16_t getDataAsWord();
        uint32_t getDataAsLong();
        virtual void write(std::vector<uint8_t> data);
        virtual void write(uint8_t data);
        virtual void write(uint16_t data);
        virtual void write(uint32_t data);
    };

    class AddressingMode {
    protected:
        uint8_t cycles;
        uint8_t longCycles;
        M68kCpu* cpu;
        Bus* bus;
    public:
        /* Constants */
        static BitMask<uint16_t> EA_MODE_MASK;        // Mask for getting the effective address mode from the byteOpWord.
        static BitMask<uint16_t> EA_REG_MASK;         // Mask for getting the effective address reg from the byteOpWord.
        static const uint8_t USP_ADDRESS = 0x07;            // The address register used for the user stack pointer.

        AddressingMode(M68kCpu *cpu, Bus *bus);
        virtual ~AddressingMode() = default;
        virtual uint32_t getAddress(uint8_t regAddr) = 0;
        virtual std::unique_ptr<AddressingResult> getData(uint8_t regAddr, uint8_t size);
        virtual uint8_t getModeId() = 0;
        virtual void setBus(Bus* b);
        virtual std::string disassemble(uint8_t regAddr, uint8_t size) = 0;
        virtual uint8_t getMoveCycleKey();
    };
}

