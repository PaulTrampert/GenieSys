//
// Created by paul.trampert on 11/8/2020.
//

#pragma once
#include <cstdint>
#include <array>
#include "GenieSys/enums.h"
#include "GenieSys/BriefExtensionWord.h"

class Bus;
/**
 * The CPU used in the Sega Genesis.
 */
class M68kCpu {

private:
    /* Constants */
    const uint16_t EFFECTIVE_ADDR_MASK = 0x003F; // Mask for getting the entire effective address field from the opWord.
    const uint16_t EA_MODE_MASK = 0x0038;        // Mask for getting the effective address mode from the opWord.
    const uint16_t EA_REG_MASK = 0x0007;         // Mask for getting the effective address reg from the opWord.
    const uint8_t USP_ADDRESS = 0x07;            // The address register used for the user stack pointer.

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

public:
    M68kCpu();
    ~M68kCpu();
    void ConnectBus(Bus* bus);

    /* Addressing Modes */
    /**
     * Address is set to the address of the data register containing the operand.
     */
    void dataRegisterDirect();

    /**
     * Address is set to the address of the address register containing the operand.
     */
    void addressRegisterDirect();

    /**
     * Address is set to the address contained within the specified address register.
     */
    void addressRegisterIndirect();

    /**
     * Same as addressRegisterIndirect, except the specified address register is then
     * incremented by the number of bytes for the operand.
     */
    void addressRegisterIndirectPostIncrement();

    /**
     * Same as addressRegisterIndirect, except the specified address register is first
     * decremented by the number of bytes for the operand.
     */
    void addressRegisterIndirectPreDecrement();

    /**
     * The address is set to the contents of the given address register + a 16-bit displacement.
     */
    void addressRegisterIndirectDisplacement();

    /**
     * It's complicated. Maths are involved. Then the address is set.
     */
    void addressRegisterIndirectWithIndex();

    /**
     * The address is set to the contents of the program counter + a 16-bit displacement.
     */
    void programCounterIndirectDisplacement();

    /**
     * Same insanity that is addressRegisterIndirectWithIndex, except using the program
     * counter as the base.
     */
    void programCounterIndirectWithIndex();

    /**
     * Address is the 16-bit extension word, sign-extended to 32 bits.
     */
    void absoluteShortAddressingMode();

    /**
     * Address is the next 32-bits of the program counter.
     */
    void absoluteLongAddressingMode();

    /**
     * Sets the address field to the value of the program counter.
     */
    void immediateData();
};


