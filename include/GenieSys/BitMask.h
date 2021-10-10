//
// Created by paul.trampert on 12/6/2020.
//

#pragma once
#include <cstdint>

namespace GenieSys {
    /**
         * Class to support common bit-masking operations.
         * @tparam T The type of the bitmask. Should be an unsigned integer type.
         */
    template<class T>
    class BitMask {
    private:
        T mask;
        uint8_t offset;
        uint8_t width;
        T minValue = 0;
        T maxValue;

    public:
        /**
         * Create a new BitMask with the given highest bit and width. For example, for a byte BitMask, one might say
         * highBit == 4, width == 2, which produces the mask 0b00011000.
         * @param highBit The highest bit of the mask.
         * @param width The width of the mask, extending downward from the highest bit.
         */
        BitMask(uint8_t highBit, uint8_t width) {
            this->width = width;
            mask = 0;
            for (int i = 0; i < width; i++) {
                mask |= 1 << (highBit - i);
            }
            offset = highBit - width + 1;
            maxValue = mask >> offset;
        }

        /**
         * Create a new BitMask with the given highest bit and width. For example, for a byte BitMask, one might say
         * highBit == 4, width == 2, which produces the mask 0b00011000.
         * @param highBit The highest bit of the mask.
         * @param width The width of the mask, extending downward from the highest bit.
         * @param minValue The minimum valid value of the mask.
         * @param maxValue The maximum valid value of the mask.
         */
        BitMask(uint8_t highBit, uint8_t width, T minValue, T maxValue) : BitMask(highBit, width) {
            this->minValue = minValue;
            this->maxValue = maxValue;
        }

        /**
         * Apply the bitmask to the given input to get the offset-shifted value of the described field.
         * @param input The number to apply the bitmask to.
         * @return The value of the field described by this mask.
         */
        T apply(T input) {
            return (input & mask) >> offset;
        }

        /**
         * Get the maximum valid value described by this mask.
         * @return The max value described by this mask.
         */
        T getMaxValue() {
            return maxValue;
        }

        /**
         * Get the minimum valid value of the mask.
         * @return The min value described by this mask.
         */
        T getMinValue() {
            return minValue;
        }

        /**
         * Get the width of this mask, in bits.
         * @return The width of this mask in bits.
         */
        T getWidth() {
            return width;
        }

        /**
         * Insert the bits of fieldValue into base at the location described by this mask.
         * @param base The base value to modify.
         * @param fieldValue The field value to insert.
         * @return The resulting value.
         */
        T compose(T base, T fieldValue) {
            return (base & ~mask) | (fieldValue << offset);
        }
    };
}