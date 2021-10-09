//
// Created by paul.trampert on 12/6/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/BitMask.h>


TEST(BitMaskTest, ItCorrectlyMasksForTheMiddleOfNumbers) {
    auto subject = GenieSys::BitMask<uint8_t>(6, 3);
    ASSERT_EQ(7, subject.apply(0b11111111));
}

TEST(BitMaskTest, ItCorrectlyMasksTheLowestNBits) {
    auto subject = GenieSys::BitMask<uint8_t>(2, 3);
    ASSERT_EQ(7, subject.apply(0b11111111));
}

TEST(BitMaskTest, ItCorrectlyMasksTheHighestNBits) {
    auto subject = GenieSys::BitMask<uint8_t>(7, 3);
    ASSERT_EQ(7, subject.apply(0b11111111));
}

TEST(BitMaskTest, ItCorrectlyGetsTheMaxValue) {
    auto subject = GenieSys::BitMask<uint8_t>(7, 3);
    ASSERT_EQ(7, subject.getMaxValue());
}

TEST(BitMaskTest, ItCorrectlyComposesAFieldIntoATarget) {
    auto subject = GenieSys::BitMask<uint8_t>(7, 3);
    ASSERT_EQ(0b01011111, subject.compose(0b11111111, 0b010));
}

TEST(BitMaskTest, ItCorrectlyCalculatesDefaultBounds) {
    auto subject = GenieSys::BitMask<uint8_t>(7, 2);
    ASSERT_EQ(0, subject.getMinValue());
    ASSERT_EQ(3, subject.getMaxValue());
}

TEST(BitMaskTest, BoundedConstructorCorrectlySetsBounds) {
    auto subject = GenieSys::BitMask<uint8_t>(7, 2, 1, 2);
    ASSERT_EQ(1, subject.getMinValue());
    ASSERT_EQ(2, subject.getMaxValue());
}