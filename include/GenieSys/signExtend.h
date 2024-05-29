//
// Created by paul.trampert on 11/24/2020.
//

#pragma once

namespace GenieSys {
    /**
     * Sign extends a number from a given number of bits to the size of the type
     * @tparam T The type of the number to sign extend
     * @param x The number to sign extend
     * @param bits The number of bits to sign extend from
     * @return The sign extended number
     */
    template<class T>
    T signExtend(T x, unsigned int bits) {
        T m = 1U << (bits - 1);
        return (x ^ m) - m;
    }
}