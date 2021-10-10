//
// Created by paul.trampert on 2/23/2021.
//

#pragma once

#include <sstream>
#include <cmath>
#include "./enums.h"

namespace GenieSys {
    template<class U, class S>
    static uint8_t getAdditionCcrFlags(U result, U op1, U op2) {
        uint8_t ccr = 0;
        if (result < op1) {
            ccr |= GenieSys::CCR_CARRY | GenieSys::CCR_EXTEND;
        }
        if (((S) result < 0 && (S)op1 > 0 && (S)op2 > 0) ||
            ((S) result > 0 && (S)op1 < 0 && (S)op2 < 0)
                ) {
            ccr |= GenieSys::CCR_OVERFLOW;
        }
        if (result == 0) {
            ccr |= GenieSys::CCR_ZERO;
        }
        if ((S)result < 0) {
            ccr |= GenieSys::CCR_NEGATIVE;
        }
        return ccr;
    }

    template <class U, class S>
    static uint8_t getSubtractionCcrFlags(U result, U op1, U op2) {
        uint8_t flags = 0;
        if (((S)op2 > 0 && (S)result > (S)op1)
            || ((S)op2 < 0 && (S)result < (S)op1)) {
            flags |= GenieSys::CCR_OVERFLOW;
        }
        if ((S)result < 0) {
            flags |= GenieSys::CCR_NEGATIVE;
        }
        if (result == 0) {
            flags |= GenieSys::CCR_ZERO;
        }
        if (op1 < op2) {
            flags |= GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY;
        }
        return flags;
    }

    template <class U, class S>
    static uint8_t getNegxCcrFlags(U result, U op1, U op2, uint8_t oldCcr) {
        uint8_t flags = 0;
        if (((S)op2 > 0 && (S)result > (S)op1)
            || ((S)op2 < 0 && (S)result < (S)op1)) {
            flags |= GenieSys::CCR_OVERFLOW;
        }
        if ((S)result < 0) {
            flags |= GenieSys::CCR_NEGATIVE;
        }
        if (result == 0) {
            flags |= (oldCcr & GenieSys::CCR_ZERO);
        }
        if (op1 < op2) {
            flags |= GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY;
        }
        return flags;
    }

    template <class U, class S>
    static uint8_t getNegCcrFlags(U op) {
        uint8_t flags = 0;
        if (op < 127) {
            flags |= GenieSys::CCR_NEGATIVE;
        }
        if (op == 0) {
            flags |= GenieSys::CCR_ZERO;
        }
        else {
            flags |= GenieSys::CCR_CARRY | GenieSys::CCR_EXTEND;
        }
        return flags;
    }
}

