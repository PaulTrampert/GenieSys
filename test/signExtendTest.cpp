//
// Created by paul.trampert on 11/25/2020.
//
#include <gtest/gtest.h>
#include "GenieSys/signExtend.h"
#include "signExtend.h"

TEST(GenieSys::signExtend, ItSignExtendsByteToInt) {
    uint8_t byte = 0xF0;
    int32_t result = GenieSys::signExtend<int32_t>(byte, 8);
    EXPECT_EQ(-16, result);
}

TEST(GenieSys::signExtend, ItSignExtendsInt16ToInt) {
    uint16_t uint16 = 0xFFF0;
    int32_t result = GenieSys::signExtend<int32_t>(uint16, 16);
    EXPECT_EQ(-16, result);
}