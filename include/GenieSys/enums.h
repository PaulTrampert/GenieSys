//
// Created by paul.trampert on 11/26/2020.
//

#pragma once

namespace GenieSys {
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
    enum TRAP_VECTORS {
        TV_INIT_SSP = 0,
        TV_INIT_PC = 1,
        TV_BUS_ERR = 2,
        TV_ADDR_ERR = 3,
        TV_ILLEGAL_INSTR = 4,
        TV_DIV_ZERO = 5,
        TV_CHK = 6,
        TV_TRAPV = 7,
        TV_PRIVILEGE = 8,
        TV_TRACE = 9,
        TV_UNIMPL1 = 10,
        TV_UNIMPL2 = 11,
        TV_UNINIT_INTERRUPT_VECTOR = 15,
        TV_SPURIOUS = 24,
        TV_LEVEL1_INTER = 25,
        TV_LEVEL2_INTER = 26,
        TV_LEVEL3_INTER = 27,
        TV_LEVEL4_INTER = 28,
        TV_LEVEL5_INTER = 29,
        TV_LEVEL6_INTER = 30,
        TV_LEVEL7_INTER = 31,
        TV_TRAP0 = 32,
        TV_TRAP1 = 33,
        TV_TRAP2 = 34,
        TV_TRAP3 = 35,
        TV_TRAP4 = 36,
        TV_TRAP5 = 37,
        TV_TRAP6 = 38,
        TV_TRAP7 = 39,
        TV_TRAP8 = 40,
        TV_TRAP9 = 41,
        TV_TRAP10 = 42,
        TV_TRAP11 = 43,
        TV_TRAP12 = 44,
        TV_TRAP13 = 45,
        TV_TRAP14 = 46,
        TV_TRAP15 = 47,
        TV_USER_INTERRUPT = 64
    };
}

