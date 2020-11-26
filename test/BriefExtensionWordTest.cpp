//
// Created by paul.trampert on 11/26/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/BriefExtensionWord.h>

TEST(BriefExtensionWord, ItDecodesCorrectly) {
    uint16_t word = 0b1010101000000011;
    auto result = decodeBriefExtensionWord(word);
    EXPECT_EQ(M68K_REG_TYPE_ADDR, result.idxRegType);
    EXPECT_EQ((uint8_t)2, result.idxRegAddr);
    EXPECT_EQ(EXT_WORD_IDX_SIZE_LONG_WORD, result.idxSize);
    EXPECT_EQ(1, result.scale);
    EXPECT_EQ(3, result.displacement);
}