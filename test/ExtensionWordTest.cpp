//
// Created by paul.trampert on 11/26/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/ExtensionWord.h>

TEST(ExtensionWord, DecodeBriefExtensionWord) {
    uint16_t word = 0b1010101000000011;
    auto result = ExtensionWord(word);
    EXPECT_EQ(M68K_REG_TYPE_ADDR, result.getIdxRegType());
    EXPECT_EQ((uint8_t)2, result.getIdxRegAddr());
    EXPECT_EQ(EXT_WORD_IDX_SIZE_LONG_WORD, result.getIdxSize());
    EXPECT_EQ(1, result.getScale());
    EXPECT_TRUE(result.isBrief());
    EXPECT_EQ(3, result.getDisplacement());
}

TEST(ExtensionWord, DecodeExtensionWord) {
    uint16_t word = 0b1010101110100011;
    auto result = ExtensionWord(word);
    EXPECT_EQ(M68K_REG_TYPE_ADDR, result.getIdxRegType());
    EXPECT_EQ((uint8_t)2, result.getIdxRegAddr());
    EXPECT_EQ(EXT_WORD_IDX_SIZE_LONG_WORD, result.getIdxSize());
    EXPECT_EQ(1, result.getScale());
    EXPECT_FALSE(result.isBrief());
    EXPECT_EQ(true, result.getBaseRegSuppress());
    EXPECT_EQ(false, result.getIndexSuppress());
    EXPECT_EQ(2, result.getBaseDisplacementSize());
    EXPECT_EQ(3, result.getIndexIndirectSelection());
}