//
// Created by paul.trampert on 2/23/2021.
//

#pragma once

#include <GenieSys/CpuOperations/ADDI.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/getCcrFlags.h>
#include <sstream>
#include <cmath>
#include <GenieSys/CpuOperations/SUBI.h>

template<class U, class S>
static uint8_t getAdditionCcrFlags(U result, U op1, U op2) {
    uint8_t ccr = 0;
    if (result < op1) {
        ccr |= CCR_CARRY | CCR_EXTEND;
    }
    if (((S) result < 0 && (S)op1 > 0 && (S)op2 > 0) ||
        ((S) result > 0 && (S)op1 < 0 && (S)op2 < 0)
            ) {
        ccr |= CCR_OVERFLOW;
    }
    if (result == 0) {
        ccr |= CCR_ZERO;
    }
    if ((S)result < 0) {
        ccr |= CCR_NEGATIVE;
    }
    return ccr;
}

template <class U, class S>
static uint8_t getSubtractionCcrFlags(U result, U op1, U op2) {
    uint8_t flags = 0;
    if (((S)op2 > 0 && (S)result > (S)op1)
        || ((S)op2 < 0 && (S)result < (S)op1)) {
        flags |= CCR_OVERFLOW;
    }
    if ((S)result < 0) {
        flags |= CCR_NEGATIVE;
    }
    if (result == 0) {
        flags |= CCR_ZERO;
    }
    if (op1 < op2) {
        flags |= CCR_EXTEND | CCR_CARRY;
    }
    return flags;
}

template <class U, class S>
static uint8_t getNegxCcrFlags(U result, U op1, U op2, uint8_t oldCcr) {
    uint8_t flags = 0;
    if (((S)op2 > 0 && (S)result > (S)op1)
        || ((S)op2 < 0 && (S)result < (S)op1)) {
        flags |= CCR_OVERFLOW;
    }
    if ((S)result < 0) {
        flags |= CCR_NEGATIVE;
    }
    if (result == 0) {
        flags |= (oldCcr & CCR_ZERO);
    }
    if (op1 < op2) {
        flags |= CCR_EXTEND | CCR_CARRY;
    }
    return flags;
}

template <class U, class S>
static uint8_t getNegCcrFlags(U op) {
    uint8_t flags = 0;
    if (op < 127) {
        flags |= CCR_NEGATIVE;
    }
    if (op == 0) {
        flags |= CCR_ZERO;
    }
    else {
        flags |= CCR_CARRY | CCR_EXTEND;
    }
    return flags;
}