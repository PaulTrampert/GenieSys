//
// Created by paul.trampert on 12/6/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/BitMask.h>

TEST(BitMaskTest, ItCorrectlyMasksForTheMiddleOfNumbers) {
    auto subject = BitMask<uint8_t>(6, 3);
    ASSERT_EQ(7, subject.apply(0b11111111));
}

TEST(BitMaskTest, ItCorrectlyMasksTheLowestNBits) {
    auto subject = BitMask<uint8_t>(2, 3);
    ASSERT_EQ(7, subject.apply(0b11111111));
}

TEST(BitMaskTest, ItCorrectlyMasksTheHighestNBits) {
    auto subject = BitMask<uint8_t>(7, 3);
    ASSERT_EQ(7, subject.apply(0b11111111));
}