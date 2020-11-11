//
// Created by paul.trampert on 11/8/2020.
//

#pragma once
#include <cstdint>
#include <array>

class Bus;

class m68000cpu {

private:
    Bus* bus = nullptr;

    std::array<uint32_t, 8> dataRegisters;
    std::array<uint32_t, 8> addressRegisters;
    uint32_t pc = 0;

    uint8_t ccr = 0;

    uint8_t read(uint32_t addr);
    void write(uint32_t addr, uint8_t data);

    enum CCR_FLAGS {
        CCR_EXTEND = 1 << 4, // Extend - Set to the value of the C-bit for arithmetic operations
        CCR_NEGATIVE = 1 << 3, // Negative - Set if the most significant bit of the result is set.
        CCR_ZERO = 1 << 2, // Zero - Set if the result equals zero
        CCR_OVERFLOW = 1 << 1, // Overflow - Set if an arithmetic overflow occurs
        CCR_CARRY = 1       // Carry - Set if a carry out of the most significant bit of the operand occurs for an addition
    };

    std::array<long double, 8> fpDataRegisters;
    uint8_t fpExceptionEnableByte = 0;
    uint8_t fpModeControl = 0;
    uint8_t fpConditionCode = 0;
    uint8_t fpQuotient = 0;
    uint8_t fpExceptionStatus = 0;
    uint8_t fpAccruedException = 0;
    uint32_t fpInstructionAddrRegister = 0;

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
public:
    m68000cpu();
    ~m68000cpu();
    void ConnectBus(Bus* bus);
};


