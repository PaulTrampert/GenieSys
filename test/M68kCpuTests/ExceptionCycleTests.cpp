//
// Created by paul on 9/6/25.
//
#include <gtest/gtest.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/enums.h>

using namespace GenieSys;

// Exception processing times, from the exception processing timing table. Most vectors cost 34
// cycles, but a handful of them are documented separately.
struct ExceptionCycleTestParams {
    std::string testName;
    uint8_t vector;
    uint8_t expectedCycles;
};

struct ExceptionCycleTest : testing::TestWithParam<ExceptionCycleTestParams> {};

TEST_P(ExceptionCycleTest, GetExceptionCycles) {
    ASSERT_EQ(GetParam().expectedCycles, M68kCpu::getExceptionCycles(GetParam().vector));
}

INSTANTIATE_TEST_SUITE_P(M68kCpu, ExceptionCycleTest, testing::Values(
        ExceptionCycleTestParams { .testName = "BusError", .vector = TV_BUS_ERR, .expectedCycles = 50 },
        ExceptionCycleTestParams { .testName = "AddressError", .vector = TV_ADDR_ERR, .expectedCycles = 50 },
        ExceptionCycleTestParams { .testName = "IllegalInstruction", .vector = TV_ILLEGAL_INSTR, .expectedCycles = 34 },
        ExceptionCycleTestParams { .testName = "DivideByZero", .vector = TV_DIV_ZERO, .expectedCycles = 42 },
        ExceptionCycleTestParams { .testName = "Chk", .vector = TV_CHK, .expectedCycles = 40 },
        ExceptionCycleTestParams { .testName = "Trapv", .vector = TV_TRAPV, .expectedCycles = 34 },
        ExceptionCycleTestParams { .testName = "PrivilegeViolation", .vector = TV_PRIVILEGE, .expectedCycles = 34 },
        ExceptionCycleTestParams { .testName = "Trace", .vector = TV_TRACE, .expectedCycles = 34 },
        ExceptionCycleTestParams { .testName = "SpuriousInterrupt", .vector = TV_SPURIOUS, .expectedCycles = 44 },
        ExceptionCycleTestParams { .testName = "Level1Interrupt", .vector = TV_LEVEL1_INTER, .expectedCycles = 44 },
        ExceptionCycleTestParams { .testName = "Level7Interrupt", .vector = TV_LEVEL7_INTER, .expectedCycles = 44 },
        ExceptionCycleTestParams { .testName = "Trap0", .vector = TV_TRAP0, .expectedCycles = 34 },
        ExceptionCycleTestParams { .testName = "UninitialisedInterruptVector", .vector = TV_UNINIT_INTERRUPT_VECTOR, .expectedCycles = 34 }
    ),
    [](const testing::TestParamInfo<ExceptionCycleTestParams>& info) {
        return info.param.testName;
    }
);
