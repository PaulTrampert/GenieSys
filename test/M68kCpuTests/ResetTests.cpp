//
// Created by pault on 12/29/2021.
//

#include <gtest/gtest.h>
#include "./WithM68kCpu.h"

struct ResetTestParams {
    uint32_t vectorValue;
    uint32_t initPc;
};

struct ResetTests : public WithM68KCpu, public testing::TestWithParam<ResetTestParams> {

};

TEST_P(ResetTests, Reset) {
    auto params = GetParam();
    EXPECT_CALL(bus, readLong(GenieSys::TV_INIT_PC * 4))
        .Times(1)
        .WillOnce(testing::Return(params.vectorValue));
    subject->setPc(params.initPc);

    subject->reset();

    ASSERT_EQ(params.vectorValue, subject->getPc());
}

INSTANTIATE_TEST_SUITE_P(M68kCpuTests, ResetTests,
                         testing::Values(
                                 ResetTestParams{
                                     .vectorValue = 456,
                                     .initPc = 9000
                                 },
                                 ResetTestParams{
                                         .vectorValue = 0xFFFFFFFF,
                                         .initPc = 0
                                 },
                                 ResetTestParams{
                                         .vectorValue = 0xABCDEF,
                                         .initPc = 42
                                 },
                                 ResetTestParams{
                                         .vectorValue = 0x01100110,
                                         .initPc = 369
                                 }
                                 ));