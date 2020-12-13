//
// Created by paul.trampert on 12/11/2020.
//

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <GenieSys/getPossibleOpcodes.h>

TEST(getPossibleOpcodes, ItGetsThePossibleOpcodesForOneField) {
    auto mask = BitMask<uint16_t>(9, 2);
    auto result = getPossibleOpcodes((uint16_t) 0b1111110011111111, &mask);
    ASSERT_EQ(4, result.size());
    ASSERT_THAT(result, ::testing::ElementsAre(0b1111110011111111, 0b1111110111111111, 0b1111111011111111, 0b1111111111111111));
}

TEST(getPossibleOpcodes, ItGetsThePossibleOpcodesForTwoFields) {
    auto mask1 = BitMask<uint16_t>(9, 1);
    auto mask2 = BitMask<uint16_t>(8, 1);
    std::vector<BitMask<uint16_t>*> masks {
        &mask1,
        &mask2
    };
    auto result = getPossibleOpcodes((uint16_t) 0b1111110011111111, masks);
    ASSERT_EQ(4, result.size());
    ASSERT_THAT(result, ::testing::ElementsAre(0b1111110011111111, 0b1111110111111111, 0b1111111011111111, 0b1111111111111111));
}