//
// Created by paul on 8/16/25.
//
#include <string>
#include <cstdint>
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ADDQ.h>
#include "../MockAddressingMode.h"
#include "../MockBus.h"
#include "../MockCpu.h"
#include "GenieSys/AddressingModes/DataRegisterDirectMode.h"
#include "GenieSys/AddressingModes/AddressRegisterDirectMode.h"
#include "GenieSys/AddressingModes/AddressRegisterIndirectMode.h"

using namespace GenieSys;

struct ADDQTestParams {
    std::string testName;
    uint32_t registerData;
    uint8_t data;
    uint8_t eaMode;
    uint8_t eaReg;
    uint8_t eaCycles;
    uint8_t size;
    uint8_t expectedEffectiveSize;
    uint8_t expectedCycles;
    uint32_t expectedResult;
    uint8_t writeByteCalls;
    uint8_t writeWordCalls;
    uint8_t writeLongCalls;
    std::string expectedDisassembly;
};

class ADDQTest : public ::testing::TestWithParam<ADDQTestParams> {
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
    ADDQ* subject;

    ADDQTest() {
        subject = new ADDQ(cpu, bus);
        ON_CALL(*cpu, getAddressingMode(testing::_))
            .WillByDefault(testing::Return(addressingMode));
        ON_CALL(*addressingMode, getDataProxy(testing::_, testing::_))
            .WillByDefault(testing::Return(addressingResult));
        testing::Mock::AllowLeak(addressingResult);
    }

    ~ADDQTest() override {
        delete subject;
        delete cpu;
        delete bus;
        delete addressingMode;
    }
};

TEST_P(ADDQTest, Execute) {
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
    EXPECT_CALL(*addressingMode, getDataProxy(params.eaReg, params.expectedEffectiveSize))
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

TEST_P(ADDQTest, Disassemble) {
    const auto& params = GetParam();
    uint16_t opWord = dataMask.compose(baseOpWord, params.data);
    opWord = sizeMask.compose(opWord, params.size);
    opWord = eaModeMask.compose(opWord, params.eaMode);
    opWord = eaRegMask.compose(opWord, params.eaReg);
    uint8_t numBytes = 1 << params.size;
    ON_CALL(*addressingMode, disassemble(params.eaReg, numBytes))
        .WillByDefault(testing::Return("EA" + std::to_string(params.eaMode) + "-" + std::to_string(params.eaReg)));

    std::string disassembly = subject->disassemble(opWord);

    ASSERT_EQ(disassembly, params.expectedDisassembly);
}

INSTANTIATE_TEST_SUITE_P(ADDQ, ADDQTest, testing::Values(
        ADDQTestParams {
            .testName = "ADDQ_B_8_D0",
            .registerData = 50,
            .data = 0,
            .eaMode = GenieSys::DataRegisterDirectMode::MODE_ID,
            .eaReg = 0,
            .eaCycles = 0,
            .size = 0,
            .expectedEffectiveSize = 1,
            .expectedCycles = 4,
            .expectedResult = 58,
            .writeByteCalls = 1,
            .writeWordCalls = 0,
            .writeLongCalls = 0,
            .expectedDisassembly = "ADDQ.b #8, EA0-0"
        },
        ADDQTestParams {
            .testName = "ADDQ_B_4_D0",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::DataRegisterDirectMode::MODE_ID,
            .eaReg = 0,
            .eaCycles = 0,
            .size = 0,
            .expectedEffectiveSize = 1,
            .expectedCycles = 4,
            .expectedResult = 54,
            .writeByteCalls = 1,
            .writeWordCalls = 0,
            .writeLongCalls = 0,
            .expectedDisassembly = "ADDQ.b #4, EA0-0"
        },
        ADDQTestParams {
            .testName = "ADDQ_B_4_A0",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::AddressRegisterDirectMode::MODE_ID,
            .eaReg = 0,
            .eaCycles = 0,
            .size = 0,
            .expectedEffectiveSize = 4,
            .expectedCycles = 8,
            .expectedResult = 54,
            .writeByteCalls = 0,
            .writeWordCalls = 0,
            .writeLongCalls = 1,
            .expectedDisassembly = "ADDQ.b #4, EA1-0"
        },
        ADDQTestParams {
            .testName = "ADDQ_W_4_D3",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::DataRegisterDirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 0,
            .size = 1,
            .expectedEffectiveSize = 2,
            .expectedCycles = 4,
            .expectedResult = 54,
            .writeByteCalls = 0,
            .writeWordCalls = 1,
            .writeLongCalls = 0,
            .expectedDisassembly = "ADDQ.w #4, EA0-3"
        },
        ADDQTestParams {
            .testName = "ADDQ_W_4_A3",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::AddressRegisterDirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 0,
            .size = 1,
            .expectedEffectiveSize = 4,
            .expectedCycles = 8,
            .expectedResult = 54,
            .writeByteCalls = 0,
            .writeWordCalls = 0,
            .writeLongCalls = 1,
            .expectedDisassembly = "ADDQ.w #4, EA1-3"
        },
        ADDQTestParams {
            .testName = "ADDQ_L_4_D3",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::DataRegisterDirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 0,
            .size = 2,
            .expectedEffectiveSize = 4,
            .expectedCycles = 8,
            .expectedResult = 54,
            .writeByteCalls = 0,
            .writeWordCalls = 0,
            .writeLongCalls = 1,
            .expectedDisassembly = "ADDQ.l #4, EA0-3"
        },
        ADDQTestParams {
            .testName = "ADDQ_L_4_A3Indir",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::AddressRegisterIndirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 5,
            .size = 2,
            .expectedEffectiveSize = 4,
            .expectedCycles = 17,
            .expectedResult = 54,
            .writeByteCalls = 0,
            .writeWordCalls = 0,
            .writeLongCalls = 1,
            .expectedDisassembly = "ADDQ.l #4, EA2-3"
        },
        ADDQTestParams {
            .testName = "ADDQ_L_4_A3",
            .registerData = 50,
            .data = 4,
            .eaMode = GenieSys::AddressRegisterDirectMode::MODE_ID,
            .eaReg = 3,
            .eaCycles = 5,
            .size = 2,
            .expectedEffectiveSize = 4,
            .expectedCycles = 13,
            .expectedResult = 54,
            .writeByteCalls = 0,
            .writeWordCalls = 0,
            .writeLongCalls = 1,
            .expectedDisassembly = "ADDQ.l #4, EA1-3"
        }
    ),
    [](const testing::TestParamInfo<ADDQTest::ParamType>& info) {
        return info.param.testName;
    }
);