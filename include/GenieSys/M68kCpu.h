//
// Created by paul.trampert on 11/8/2020.
//

#pragma once
#include <cstdint>
#include <array>
#include "GenieSys/enums.h"
#include "GenieSys/BriefExtensionWord.h"

class Bus;
class AddressingMode;

/**
 * The CPU used in the Sega Genesis.
 */
class M68kCpu {

private:
    /* Private fields */
    /**
     * The 32-bit addressable bus.
     */
    Bus* bus = nullptr;

    /**
     * Data registers
     */
    std::array<uint32_t, 8> dataRegisters;

    /**
     * Address registers
     */
    std::array<uint32_t, 8> addressRegisters;

    /**
     * Program counter.
     */
    uint32_t pc = 0;

    /**
     * Condition code register
     */
    uint8_t ccr = 0;

    /**
     * Floating point data registers.
     */
    std::array<long double, 8> fpDataRegisters;
    uint8_t fpExceptionEnableByte = 0;
    uint8_t fpModeControl = 0;
    uint8_t fpConditionCode = 0;
    uint8_t fpQuotient = 0;
    uint8_t fpExceptionStatus = 0;
    uint8_t fpAccruedException = 0;
    uint32_t fpInstructionAddrRegister = 0;



    /* Operational Fields */
    uint16_t opWord;  // Single Effective Address Operation Word
    uint32_t address; // Effective address for the current operation
    DATA_SIZE operandSize;

    uint8_t clock = 0;

    /* Addressing modes */
    std::array<AddressingMode*, 8> addressingModes;

public:
    M68kCpu();
    ~M68kCpu();
    void ConnectBus(Bus* bus);
    uint16_t getCurrentOpWord();
    uint32_t getDataRegister(uint8_t addr);
    void setDataRegister(uint8_t addr, uint32_t value);
    uint32_t getAddressRegister(uint8_t addr);
    void setAddressRegister(uint8_t addr, uint32_t value);
    uint32_t getPc();
    void incrementPc(int32_t amount);
    void setPc(uint32_t value);
    DATA_SIZE getOperandSize();
    void tick();
};


