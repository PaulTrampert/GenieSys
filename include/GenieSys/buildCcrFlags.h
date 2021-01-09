//
// Created by paul.trampert on 1/8/2021.
//

#pragma once

template<class U, class S>
static uint8_t buildCcrFlags(U result, U op1, U op2) {
    uint8_t ccr = 0;
    if (result < op1) {
        ccr |= CCR_OVERFLOW;
    }
    if (((S) result < 0 && (S)op1 > 0 && (S)op2 > 0) ||
        ((S) result > 0 && (S)op1 < 0 && (S)op2 < 0)
            ) {
        ccr |= CCR_EXTEND | CCR_CARRY;
    }
    if (result == 0) {
        ccr |= CCR_ZERO;
    }
    if ((S)result < 0) {
        ccr |= CCR_NEGATIVE;
    }
    return ccr;
}