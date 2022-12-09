//
// Created by paul.trampert on 11/30/2020.
//
#include <gtest/gtest.h>
#include <vector>
#include <GenieSys/numberUtils.h>

TEST(numberUtils, bytesToByte) {
    uint8_t result = GenieSys::bytesToByte(std::vector<uint8_t>{8});
    EXPECT_EQ(8u, result);
}

TEST(numberUtils, bytesToWord) {
    uint16_t result = GenieSys::bytesToWord(std::vector<uint8_t>{0x11, 0x22});
    EXPECT_EQ(0x1122u, result);
}

TEST(numberUtils, bytesToLong) {
    uint32_t result = GenieSys::bytesToLong(std::vector<uint8_t>{0x11, 0x22, 0x33, 0x44});
    EXPECT_EQ(0x11223344u, result);
}

TEST(numberUtils, getBytesByte) {
    auto result = GenieSys::getBytes((uint8_t)8);
    EXPECT_EQ(8, result[0]);
}

TEST(numberUtils, getBytesWord) {
    auto result = GenieSys::getBytes((uint16_t)0x1122);
    EXPECT_EQ(0x11, result[0]);
    EXPECT_EQ(0x22, result[1]);
}

TEST(numberUtils, getBytesLong) {
    auto result = GenieSys::getBytes((uint32_t)0x11223344);
    EXPECT_EQ(0x11, result[0]);
    EXPECT_EQ(0x22, result[1]);
    EXPECT_EQ(0x33, result[2]);
    EXPECT_EQ(0x44, result[3]);
}

TEST(numberUtils, bitwiseReverse) {
    auto result = GenieSys::bitwiseReverse(0b1010101111010010);
    EXPECT_EQ(0b0100101111010101, result);
}