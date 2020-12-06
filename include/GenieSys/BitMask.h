//
// Created by paul.trampert on 12/6/2020.
//

#pragma once
#include <cstdint>

template<class T>
class BitMask {
private:
    T mask;
    uint8_t offset;

public:
    BitMask(uint8_t highBit, uint8_t numBits) {
        mask = 0;
        for (int i = 0; i < numBits; i++) {
            mask |= 1 << (highBit - i);
        }
        offset = highBit - numBits + 1;
    }

    T apply(T input) {
        return (input & mask) >> offset;
    }
};