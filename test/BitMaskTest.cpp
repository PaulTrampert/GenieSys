//
// Created by paul.trampert on 12/6/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/BitMask.h>

TEST(BitMaskTest, ItWorks) {
    auto subject = BitMask<uint8_t>(6, 3);
    ASSERT_EQ(0, subject.apply(0b10001111));
}