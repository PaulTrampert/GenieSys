//
// Created by paul on 5/2/25.
//
#include <gtest/gtest.h>
#include "GenieSys/ConditionCodes.h"
#include "./WithM68kCpu.h"

using namespace GenieSys;

struct TestConditionCodeTestParams {
    std::string testName;
    uint8_t conditionCode;
    uint8_t ccRegister;
    bool expectedResult;
};

struct TestConditionCodeTests :
        public WithM68KCpu,
        public testing::TestWithParam<TestConditionCodeTestParams> {

};

TEST_P(TestConditionCodeTests, TestConditionCode) {
    const auto& params = GetParam();
    subject->setCcrFlags(params.ccRegister);
    ASSERT_EQ(params.expectedResult, subject->testConditionCode(params.conditionCode));
}

INSTANTIATE_TEST_SUITE_P(M68kCpuTests, TestConditionCodeTests,
    testing::Values(
        TestConditionCodeTestParams {
            .testName = "T_WhenAlwaysTrue",
            .conditionCode = CC_T,
            .ccRegister = 0,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "F_WhenAlwaysFalse",
            .conditionCode = CC_F,
            .ccRegister = 0,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "CC_WhenCarryClear",
            .conditionCode = CC_CC,
            .ccRegister = 0,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "CC_WhenCarrySet",
            .conditionCode = CC_CC,
            .ccRegister = CCR_CARRY,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "CS_WhenCarrySet",
            .conditionCode = CC_CS,
            .ccRegister = CCR_CARRY,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "CS_WhenCarryClear",
            .conditionCode = CC_CS,
            .ccRegister = 0,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "EQ_WhenZeroSet",
            .conditionCode = CC_EQ,
            .ccRegister = CCR_ZERO,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "EQ_WhenZeroClear",
            .conditionCode = CC_EQ,
            .ccRegister = 0,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "GE_WhenNegativeAndOverflow",
            .conditionCode = CC_GE,
            .ccRegister = CCR_NEGATIVE | CCR_OVERFLOW,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "GE_WhenNegativeAndNoOverflow",
            .conditionCode = CC_GE,
            .ccRegister = CCR_NEGATIVE,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "GE_WhenPositiveAndOverflow",
            .conditionCode = CC_GE,
            .ccRegister = CCR_OVERFLOW,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "GE_WhenPositiveAndNoOverflow",
            .conditionCode = CC_GE,
            .ccRegister = 0,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "GT_WhenZero",
            .conditionCode = CC_GT,
            .ccRegister = CCR_ZERO,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "GT_WhenNegativeAndOverflow",
            .conditionCode = CC_GT,
            .ccRegister = CCR_NEGATIVE | CCR_OVERFLOW,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "GT_WhenNegativeAndNoOverflow",
            .conditionCode = CC_GT,
            .ccRegister = CCR_NEGATIVE,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "GT_WhenPositiveAndOverflow",
            .conditionCode = CC_GT,
            .ccRegister = CCR_OVERFLOW,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "GT_WhenPositiveAndNoOverflow",
            .conditionCode = CC_GT,
            .ccRegister = 0,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "HI_WhenCarryAndZeroSet",
            .conditionCode = CC_HI,
            .ccRegister = CCR_CARRY | CCR_ZERO,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "HI_WhenCarrySet",
            .conditionCode = CC_HI,
            .ccRegister = CCR_CARRY,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "HI_WhenZeroSet",
            .conditionCode = CC_HI,
            .ccRegister = CCR_ZERO,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "HI_WhenCarryAndZeroClear",
            .conditionCode = CC_HI,
            .ccRegister = 0,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "LE_WhenZero",
            .conditionCode = CC_LE,
            .ccRegister = CCR_ZERO,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "LE_WhenNegativeAndOverflow",
            .conditionCode = CC_LE,
            .ccRegister = CCR_NEGATIVE | CCR_OVERFLOW,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "LE_WhenNegativeAndNoOverflow",
            .conditionCode = CC_LE,
            .ccRegister = CCR_NEGATIVE,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "LE_WhenPositiveAndOverflow",
            .conditionCode = CC_LE,
            .ccRegister = CCR_OVERFLOW,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "LE_WhenPositiveAndNoOverflow",
            .conditionCode = CC_LE,
            .ccRegister = 0,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "LS_WhenCarrySet",
            .conditionCode = CC_LS,
            .ccRegister = CCR_CARRY,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "LS_WhenZeroSet",
            .conditionCode = CC_LS,
            .ccRegister = CCR_ZERO,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "LS_WhenCarryAndZeroSet",
            .conditionCode = CC_LS,
            .ccRegister = CCR_CARRY | CCR_ZERO,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "LS_WhenCarryAndZeroClear",
            .conditionCode = CC_LS,
            .ccRegister = 0,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "LT_WhenNegativeAndNoOverflow",
            .conditionCode = CC_LT,
            .ccRegister = CCR_NEGATIVE,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "LT_WhenPositiveAndOverflow",
            .conditionCode = CC_LT,
            .ccRegister = CCR_OVERFLOW,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "LT_WhenPositiveAndNoOverflow",
            .conditionCode = CC_LT,
            .ccRegister = 0,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "LT_WhenNegativeAndOverflow",
            .conditionCode = CC_LT,
            .ccRegister = CCR_NEGATIVE | CCR_OVERFLOW,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "MI_WhenNegativeSet",
            .conditionCode = CC_MI,
            .ccRegister = CCR_NEGATIVE,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "MI_WhenNegativeClear",
            .conditionCode = CC_MI,
            .ccRegister = 0,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "NE_WhenZeroSet",
            .conditionCode = CC_NE,
            .ccRegister = CCR_ZERO,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "NE_WhenZeroClear",
            .conditionCode = CC_NE,
            .ccRegister = 0,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "PL_WhenNegativeSet",
            .conditionCode = CC_PL,
            .ccRegister = CCR_NEGATIVE,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "PL_WhenNegativeClear",
            .conditionCode = CC_PL,
            .ccRegister = 0,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "VC_WhenOverflowSet",
            .conditionCode = CC_VC,
            .ccRegister = CCR_OVERFLOW,
            .expectedResult = false
        },
        TestConditionCodeTestParams {
            .testName = "VC_WhenOverflowClear",
            .conditionCode = CC_VC,
            .ccRegister = 0,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "VS_WhenOverflowSet",
            .conditionCode = CC_VS,
            .ccRegister = CCR_OVERFLOW,
            .expectedResult = true
        },
        TestConditionCodeTestParams {
            .testName = "VS_WhenOverflowClear",
            .conditionCode = CC_VS,
            .ccRegister = 0,
            .expectedResult = false
        }
    ),
     [](const ::testing::TestParamInfo<TestConditionCodeTestParams>& info) {
         return info.param.testName;
     }
);