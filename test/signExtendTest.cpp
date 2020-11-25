//
// Created by paul.trampert on 11/25/2020.
//
#include <gtest/gtest.h>
#include "GenieSys/signExtend.h"

TEST(signExtend, ItSignExtendsByteToInt) {
    uint8_t byte = 0xF0;
    int32_t result = signExtend<int32_t>(byte, 8);
    EXPECT_EQ(-16, result);
}

TEST(signExtend, ItSignExtendsInt16ToInt) {
    uint16_t uint16 = 0xFFF0;
    int32_t result = signExtend<int32_t>(uint16, 16);
    EXPECT_EQ(-16, result);
}