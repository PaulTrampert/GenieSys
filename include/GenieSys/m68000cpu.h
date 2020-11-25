//
// Created by paul.trampert on 11/8/2020.
//

#pragma once
#include <cstdint>
#include <array>

class Bus;
/**
 * The CPU used in the Sega Genesis.
 */
class m68000cpu {

private:
    /* Constants */
    const uint16_t EFFECTIVE_ADDR_MASK = 0x003F; // Mask for getting the entire effective address field from the opWord.
    const uint16_t EA_MODE_MASK = 0x0038;        // Mask for getting the effective address mode from the opWord.
    const uint16_t EA_REG_MASK = 0x0007;         // Mask for getting the effective address reg from the opWord.
    const uint8_t USP_ADDRESS = 0x07;            // The address register used for the user stack pointer.

    /* Enums */
    enum CCR_FLAGS {
        CCR_EXTEND = 1 << 4, // Extend - Set to the value of the C-bit for arithmetic operations
        CCR_NEGATIVE = 1 << 3, // Negative - Set if the most significant bit of the result is set.
        CCR_ZERO = 1 << 2, // Zero - Set if the result equals zero
        CCR_OVERFLOW = 1 << 1, // Overflow - Set if an arithmetic overflow occurs
        CCR_CARRY = 1       // Carry - Set if a carry out of the most significant bit of the operand occurs for an addition
    };

    enum FP_EXCEPTION_ENABLE_FLAGS {
        FP_EE_BSUN = 1 << 7,    // Branch/Set on unordered
        FP_EE_SNAN = 1 << 6,    // Signaling NaN
        FP_EE_OPERR = 1 << 5,   // Operand error
        FP_EE_OVFL = 1 << 4,    // Overflow
        FP_EE_UNFL = 1 << 3,    // Underflow
        FP_EE_DZ = 1 << 2,      // Divide by zero
        FP_EE_INEX2 = 1 << 1,   // Inexact operation
        FP_EE_INEX1 = 1         // Inexact decimal input
    };

    enum FP_CONDITION_CODES {
        FP_CC_NEGATIVE = 1 << 3,
        FP_CC_ZERO = 1 << 2,
        FP_CC_INFINITY = 1 << 1,
        FP_CC_NAN = 1
    };

    enum FP_ACCRUED_EXCEPTIONS {
        ACCR_EX_IOP = 1 << 7,    // Invalid operation
        ACCR_EX_OVFL = 1 << 6,   // Overflow
        ACCR_EX_UNFL = 1 << 5,   // Underflow
        ACCR_EX_DZ = 1 << 4,     // Divide by zero
        ACCR_EX_INEX = 1 << 3    // Inexact
    };

    enum DATA_SIZE {
        BYTE,
        WORD,
        LONG
    };

    enum M68K_REG_TYPE {
        M68K_REG_TYPE_DATA = 0,
        M68K_REG_TYPE_ADDR = 1
    };

    enum EXT_WORD_IDX_SIZE {
        EXT_WORD_IDX_SIZE_SE_WORD = 0,
        EXT_WORD_IDX_SIZE_LONG_WORD = 1
    };

    enum EXT_WORD_BD_SIZE {
        EXT_WORD_BD_SIZE_RESERVED = 0,
        EXT_WORD_BD_SIZE_NULL = 1,
        EXT_WORD_BD_SIZE_WORD = 2,
        EXT_WORD_BD_SIZE_LONG = 3,
    };

    /* Structures for internal use */
    struct BriefExtensionWord {
        M68K_REG_TYPE idxRegType;
        uint8_t idxRegAddr;
        EXT_WORD_IDX_SIZE idxSize;
        uint8_t scale;
        int8_t displacement;
    };

    struct ExtensionWord {
        M68K_REG_TYPE idxRegType;
        uint8_t idxRegAddr;
        EXT_WORD_IDX_SIZE idxSize;
        uint8_t scale;
        bool baseRegSuppress;
        bool indexSuppress;
        EXT_WORD_BD_SIZE baseDisplacementSize;
        uint8_t indexIndirectSelection;
    };

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
    m68000cpu();
    ~m68000cpu();
    void ConnectBus(Bus* bus);

private:
    /* Utility methods for internal use */
    /**
     * Decodes the brief extention word format.
     * @param word The 16-bit brief extension word.
     * @return A struct separating out the fields of the brief extention word.
     */
    BriefExtensionWord decodeBriefExtensionWord(uint16_t word);

    /**
     * Decodes the full extension word format.
     * @param word The 16-bit extension word.
     * @return A struct separating out the fields of the full extension word.
     */
    ExtensionWord decodeExtensionWord(uint16_t word);

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


