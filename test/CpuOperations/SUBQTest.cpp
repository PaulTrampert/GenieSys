//
// Created by paul on 5/3/25.
//
#include <string>
#include <cstdint>
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/SUBQ.h>
#include "../MockAddressingMode.h"
#include "../MockBus.h"
#include "../MockCpu.h"
#include "GenieSys/AddressingModes/DataRegisterDirectMode.h"
#include "GenieSys/AddressingModes/AddressRegisterDirectMode.h"
#include "GenieSys/AddressingModes/AddressRegisterIndirectMode.h"

using namespace GenieSys;

struct SUBQTestParams {
    std::string testName;
    uint32_t registerData;
    uint8_t data;
    uint8_t eaMode;
    uint8_t eaReg;
    uint8_t eaCycles;
    uint8_t size;
    uint8_t expectedCycles;
    uint32_t expectedResult;
    uint8_t writeByteCalls;
    uint8_t writeWordCalls;
    uint8_t writeLongCalls;
    std::string expectedDisassembly;
};

class SUBQTest : public ::testing::TestWithParam<SUBQTestParams> {
protected:
    uint16_t baseOpWord = 0b0101000100000000;
    BitMask<uint16_t> dataMask = BitMask<uint16_t>(11, 3);
    BitMask<uint16_t> sizeMask = BitMask<uint16_t>(7, 2);
    BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
    BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);
    MockBus* bus = new MockBus();
    MockCpu* cpu = new MockCpu();
    MockAddressingMode* addressingMode = new MockAddressingMode();
    MockAddressingResult* addressingResult = new MockAddressingResult();
    SUBQ* subject;

    SUBQTest() {
        subject = new SUBQ(cpu, bus);
        ON_CALL(*cpu, getAddressingMode(testing::_))
            .WillByDefault(testing::Return(addressingMode));
        ON_CALL(*addressingMode, getDataProxy(testing::_, testing::_))
            .WillByDefault(testing::Return(addressingResult));
    }

    ~SUBQTest() override {
        delete subject;
        delete cpu;
        delete bus;
        delete addressingMode;
    }
};

TEST_P(SUBQTest, Execute) {
    const auto& params = GetParam();
    ON_CALL(*addressingResult, getDataAsByte())
        .WillByDefault(testing::Return(params.registerData));
    ON_CALL(*addressingResult, getDataAsWord())
        .WillByDefault(testing::Return(params.registerData));
    ON_CALL(*addressingResult, getDataAsLong())
        .WillByDefault(testing::Return(params.registerData));
    ON_CALL(*addressingResult, getCycles())
        .WillByDefault(testing::Return(params.eaCycles));
    ON_CALL(*addressingMode, getModeId())
        .WillByDefault(testing::Return(params.eaMode));

    EXPECT_CALL(*cpu, getAddressingMode(params.eaMode))
            .Times(1);
    EXPECT_CALL(*addressingMode, getDataProxy(params.eaReg, pow(2, params.size)))
            .Times(1);
    EXPECT_CALL(*addressingResult, write(testing::Matcher<uint8_t>(params.expectedResult)))
            .Times(params.writeByteCalls);
    EXPECT_CALL(*addressingResult, write(testing::Matcher<uint16_t>(params.expectedResult)))
            .Times(params.writeWordCalls);
    EXPECT_CALL(*addressingResult, write(testing::Matcher<uint32_t>(params.expectedResult)))
            .Times(params.writeLongCalls);

    uint16_t opWord = dataMask.compose(baseOpWord, params.data);
    opWord = sizeMask.compose(opWord, params.size);
    opWord = eaModeMask.compose(opWord, params.eaMode);
    opWord = eaRegMask.compose(opWord, params.eaReg);

    uint8_t cycles = subject->execute(opWord);

    ASSERT_EQ(cycles, params.expectedCycles);
}

INSTANTIATE_TEST_SUITE_P(SUBQ, SUBQTest, testing::Values(
        SUBQTestParams {
            .testName = "SUBQ_B_8_D0",
            .registerData = 50,
            .data = 0,
            .eaMode = GenieSys::DataRegisterDirectMode::MODE_ID,
            .eaReg = 0,
            .eaCycles = 0,
            .size = 0,
            .expectedCycles = 4,
            .expectedResult = 42,
            .writeByteCalls = 1,
            .writeWordCalls = 0,
            .writeLongCalls = 0
        },
        SUBQTestParams {
            .testName = "SUBQ_B_4_D0",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::DataRegisterDirectMode::MODE_ID,
            .eaReg = 0,
            .eaCycles = 0,
            .size = 0,
            .expectedCycles = 4,
            .expectedResult = 46,
            .writeByteCalls = 1,
            .writeWordCalls = 0,
            .writeLongCalls = 0
        },
        SUBQTestParams {
            .testName = "SUBQ_B_4_A0",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::AddressRegisterDirectMode::MODE_ID,
            .eaReg = 0,
            .eaCycles = 0,
            .size = 0,
            .expectedCycles = 8,
            .expectedResult = 46,
            .writeByteCalls = 0,
            .writeWordCalls = 1,
            .writeLongCalls = 0
        },
        SUBQTestParams {
            .testName = "SUBQ_W_4_D3",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::DataRegisterDirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 0,
            .size = 1,
            .expectedCycles = 4,
            .expectedResult = 46,
            .writeByteCalls = 0,
            .writeWordCalls = 1,
            .writeLongCalls = 0
        },
        SUBQTestParams {
            .testName = "SUBQ_W_4_A3",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::AddressRegisterDirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 0,
            .size = 1,
            .expectedCycles = 8,
            .expectedResult = 46,
            .writeByteCalls = 0,
            .writeWordCalls = 1,
            .writeLongCalls = 0
        },
        SUBQTestParams {
            .testName = "SUBQ_L_4_D3",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::DataRegisterDirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 0,
            .size = 2,
            .expectedCycles = 8,
            .expectedResult = 46,
            .writeByteCalls = 0,
            .writeWordCalls = 0,
            .writeLongCalls = 1
        },
        SUBQTestParams {
            .testName = "SUBQ_L_4_A3Indir",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::AddressRegisterIndirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 5,
            .size = 2,
            .expectedCycles = 17,
            .expectedResult = 46,
            .writeByteCalls = 0,
            .writeWordCalls = 0,
            .writeLongCalls = 1
        },
        SUBQTestParams {
            .testName = "SUBQ_L_4_A3",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::AddressRegisterDirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 5,
            .size = 2,
            .expectedCycles = 13,
            .expectedResult = 46,
            .writeByteCalls = 0,
            .writeWordCalls = 0,
            .writeLongCalls = 1
        }
    ),
    [](const testing::TestParamInfo<SUBQTest::ParamType>& info) {
        return info.param.testName;
    }
);