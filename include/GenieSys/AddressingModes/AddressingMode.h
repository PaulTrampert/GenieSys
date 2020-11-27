//
// Created by paul.trampert on 11/26/2020.
//

#pragma once
#include <cstdint>
#include "GenieSys/M68kCpu.h"
#include "GenieSys/Bus.h"

class AddressingMode {
protected:
    /* Constants */
    const uint16_t EFFECTIVE_ADDR_MASK = 0x003F; // Mask for getting the entire effective address field from the opWord.
    const uint16_t EA_MODE_MASK = 0x0038;        // Mask for getting the effective address mode from the opWord.
    const uint16_t EA_REG_MASK = 0x0007;         // Mask for getting the effective address reg from the opWord.
    const uint8_t USP_ADDRESS = 0x07;            // The address register used for the user stack pointer.

    M68kCpu* cpu;
    Bus* bus;
public:
    AddressingMode(M68kCpu *cpu, Bus *bus);
    virtual ~AddressingMode() = default;
    virtual uint32_t getAddress() = 0;
    virtual uint8_t getModeId() = 0;
    virtual void setBus(Bus* b);
};
