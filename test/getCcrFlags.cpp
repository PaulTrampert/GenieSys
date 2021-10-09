//
// Created by pault on 10/6/2021.
//
#include <GenieSys/getCcrFlags.h>
#include <gtest/gtest.h>

TEST(getCcrFlags, getAdditionOverflow_OverflowOccurs) {
    int8_t op1 = 127;
    int8_t op2 = 10;
    int8_t result = op1 + op2;
    uint8_t flags = GenieSys::getAdditionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(CCR_OVERFLOW, flags & CCR_OVERFLOW);
}

TEST(getCcrFlags, getAdditionOverflow_NoOverflowOccurs) {
    int8_t op1 = 27;
    int8_t op2 = 10;
    int8_t result = op1 + op2;
    uint8_t flags = GenieSys::getAdditionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(0, flags & CCR_OVERFLOW);
}

TEST(getCcrFlags, getAdditionCarry_CarryOccurs) {
    uint8_t op1 = 255;
    uint8_t op2 = 1;
    uint8_t result = op1 + op2;
    uint8_t flags = GenieSys::getAdditionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(CCR_CARRY | CCR_EXTEND, flags & (CCR_CARRY | CCR_EXTEND));
}

TEST(getCcrFlags, getAdditionCarry_NoCarryOccurs) {
    uint8_t op1 = 254;
    uint8_t op2 = 1;
    uint8_t result = op1 + op2;
    uint8_t flags = GenieSys::getAdditionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(0, flags & (CCR_CARRY | CCR_EXTEND));
}

TEST(getCcrFlags, getAdditionZero_ResultIsZero) {
    uint8_t op1 = 255;
    uint8_t op2 = 1;
    uint8_t result = op1 + op2;
    uint8_t flags = GenieSys::getAdditionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(CCR_ZERO, flags & (CCR_ZERO));
}

TEST(getCcrFlags, getAdditionZero_ResultIsNotZero) {
    uint8_t op1 = 254;
    uint8_t op2 = 1;
    uint8_t result = op1 + op2;
    uint8_t flags = GenieSys::getAdditionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(0, flags & (CCR_ZERO));
}

TEST(getCcrFlags, getAdditionNegative_ResultIsNegative) {
    uint8_t op1 = 254;
    uint8_t op2 = 1;
    uint8_t result = op1 + op2;
    uint8_t flags = GenieSys::getAdditionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(CCR_NEGATIVE, flags & (CCR_NEGATIVE));
}

TEST(getCcrFlags, getAdditionNegative_ResultIsPositive) {
    uint8_t op1 = 1;
    uint8_t op2 = 1;
    uint8_t result = op1 + op2;
    uint8_t flags = GenieSys::getAdditionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(0, flags & (CCR_NEGATIVE));
}

TEST(getCcrFlags, getSubtractionOverflow_OverflowOccurs) {
    int8_t op1 = -128;
    int8_t op2 = 1;
    int8_t result = op1 - op2;
    uint8_t flags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(CCR_OVERFLOW, flags & CCR_OVERFLOW);
}

TEST(getCcrFlags, getSubtractionOverflow_NoOverflowOccurs) {
    int8_t op1 = 27;
    int8_t op2 = 10;
    int8_t result = op1 - op2;
    uint8_t flags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(0, flags & CCR_OVERFLOW);
}

TEST(getCcrFlags, getSubtractionCarry_CarryOccurs) {
    uint8_t op1 = 5;
    uint8_t op2 = 10;
    uint8_t result = op1 - op2;
    uint8_t flags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(CCR_CARRY | CCR_EXTEND, flags & (CCR_CARRY | CCR_EXTEND));
}

TEST(getCcrFlags, getSubtractionCarry_NoCarryOccurs) {
    uint8_t op1 = 10;
    uint8_t op2 = 1;
    uint8_t result = op1 - op2;
    uint8_t flags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(0, flags & (CCR_CARRY | CCR_EXTEND));
}

TEST(getCcrFlags, getSubtractionZero_ResultIsZero) {
    uint8_t op1 = 1;
    uint8_t op2 = 1;
    uint8_t result = op1 - op2;
    uint8_t flags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(CCR_ZERO, flags & (CCR_ZERO));
}

TEST(getCcrFlags, getSubtractionZero_ResultIsNotZero) {
    uint8_t op1 = 2;
    uint8_t op2 = 1;
    uint8_t result = op1 - op2;
    uint8_t flags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(0, flags & (CCR_ZERO));
}

TEST(getCcrFlags, getSubtractionNegative_ResultIsNegative) {
    uint8_t op1 = 254;
    uint8_t op2 = 1;
    uint8_t result = op1 - op2;
    uint8_t flags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(CCR_NEGATIVE, flags & (CCR_NEGATIVE));
}

TEST(getCcrFlags, getSubtractionNegative_ResultIsPositive) {
    uint8_t op1 = 5;
    uint8_t op2 = 1;
    uint8_t result = op1 - op2;
    uint8_t flags = GenieSys::getSubtractionCcrFlags<uint8_t, int8_t>(result, op1, op2);
    ASSERT_EQ(0, flags & (CCR_NEGATIVE));
}