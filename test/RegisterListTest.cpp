//
// Created by paul on 5/19/24.
//
#include <gtest/gtest.h>
#include <GenieSys/RegisterList.h>
#include <GenieSys/numberUtils.h>

struct RegisterListTest : testing::Test {
    uint16_t regList = 0b1110101100011100;
};

TEST_F(RegisterListTest, toString) {
    auto subject = GenieSys::RegisterList(regList, false);
    ASSERT_EQ("D2-D4/A0/A1/A3/A5-A7", subject.toString());
}

TEST_F(RegisterListTest, toStringReversed) {
    auto subject = GenieSys::RegisterList(GenieSys::bitwiseReverse(regList), true);
    ASSERT_EQ("D2-D4/A0/A1/A3/A5-A7", subject.toString());
}