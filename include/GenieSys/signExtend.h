//
// Created by paul.trampert on 11/24/2020.
//

#pragma once

template<class T>
T signExtend(T x, unsigned int bits) {
    T m = 1U << (bits - 1);
    return (x ^ m) - m;
}