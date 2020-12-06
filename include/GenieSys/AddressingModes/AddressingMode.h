//
// Created by paul.trampert on 11/26/2020.
//

#pragma once
#include <cstdint>
#include "GenieSys/M68kCpu.h"
#include "GenieSys/Bus.h"
#include "GenieSys/BitMask.h"

class AddressingMode {
protected:

    M68kCpu* cpu;
    Bus* bus;
public:
    /* Constants */
    static BitMask<uint16_t> EA_MODE_MASK;        // Mask for getting the effective address mode from the opWord.
    static BitMask<uint16_t> EA_REG_MASK;         // Mask for getting the effective address reg from the opWord.
    static const uint8_t USP_ADDRESS = 0x07;            // The address register used for the user stack pointer.

    AddressingMode(M68kCpu *cpu, Bus *bus);
    virtual ~AddressingMode() = default;
    virtual uint32_t getAddress(uint8_t regAddr) = 0;
    virtual std::vector<uint8_t> getData(uint8_t regAddr, uint8_t size);
    virtual uint8_t getModeId() = 0;
    virtual void setBus(Bus* b);
};
