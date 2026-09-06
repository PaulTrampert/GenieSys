//
// Created by paul on 3/17/22.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/JSR.h>
#include <GenieSys/BitMask.h>
#include <GenieSys/enums.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectWithIndexMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPostIncrementMode.h>
#include <GenieSys/AddressingModes/AbsoluteShortAddressingMode.h>
#include <GenieSys/AddressingModes/AbsoluteLongAddressingMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectWithIndexMode.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include "../MockCpu.h"
#include "../MockBus.h"
#include "../MockAddressingMode.h"

using namespace GenieSys;
using namespace testing;

struct JSRTestParam {
    std::string testName;
    uint8_t eaModeId;
    uint8_t eaReg;
    uint32_t eaData;
    uint32_t initPc;
    // JSR is documented with a complete execution time per addressing mode rather than a base
    // plus an effective address calculation, so the addressing mode's own cycles never apply.
    uint8_t expectedCycles;
    bool expectTrap;
    std::string expectedDisassembly;
};

class JSRTest : public TestWithParam<JSRTestParam> {
public:
    uint16_t opWordBase = 0b0100111010000000;
    BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5,3);
    BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    MockCpu *cpu;
    MockBus *bus;
    MockAddressingMode *addressingMode;
    MockAddressingResult *addressingResult;
    JSR *subject;

    JSRTest() : TestWithParam() {
        cpu = new MockCpu();
        bus = new MockBus();
        addressingMode = new MockAddressingMode();
        addressingResult = new MockAddressingResult();
        subject = new JSR(cpu, bus);
    }

    ~JSRTest() override {
        delete subject;
        delete cpu;
        delete bus;
        delete addressingMode;
    }
};

TEST_P(JSRTest, Execute) {
    auto param = GetParam();
    uint16_t opWord = opWordBase;
    opWord = eaModeMask.compose(opWord, param.eaModeId);
    opWord = eaRegMask.compose(opWord, param.eaReg);
    Mock::AllowLeak(addressingResult);

    if (param.expectTrap) {
        ON_CALL(*cpu, trap(TV_ILLEGAL_INSTR)).WillByDefault(Return(param.expectedCycles));
        EXPECT_CALL(*cpu, trap(TV_ILLEGAL_INSTR));
        EXPECT_CALL(*cpu, getAddressingMode(_)).Times(0);
        EXPECT_CALL(*cpu, stackPushLong(_)).Times(0);
        EXPECT_CALL(*cpu, setPc(_)).Times(0);
        delete addressingResult;
    }
    else {
        ON_CALL(*cpu, getAddressingMode(param.eaModeId))
            .WillByDefault(Return(addressingMode));
        ON_CALL(*cpu, getPc())
            .WillByDefault(Return(param.initPc));
        ON_CALL(*addressingMode, getDataProxy(param.eaReg, 4))
            .WillByDefault(Return(addressingResult));
        ON_CALL(*addressingResult, getDataAsLong())
            .WillByDefault(Return(param.eaData));

        EXPECT_CALL(*cpu, getAddressingMode(param.eaModeId));
        EXPECT_CALL(*addressingMode, getDataProxy(param.eaReg, 4));
        EXPECT_CALL(*cpu, getPc());
        EXPECT_CALL(*cpu, stackPushLong(param.initPc));
        EXPECT_CALL(*addressingResult, getDataAsLong());
        EXPECT_CALL(*cpu, setPc(param.eaData));
        // The addressing mode's cycle count is not part of a JSR's execution time.
        EXPECT_CALL(*addressingResult, getCycles()).Times(0);
    }

    ASSERT_EQ(param.expectedCycles, subject->execute(opWord));
}

TEST_P(JSRTest, Disassemble) {
    auto param = GetParam();
    uint16_t opWord = opWordBase;
    opWord = eaModeMask.compose(opWord, param.eaModeId);
    opWord = eaRegMask.compose(opWord, param.eaReg);

    ON_CALL(*cpu, getAddressingMode(param.eaModeId))
        .WillByDefault(Return(addressingMode));
    ON_CALL(*addressingMode, disassemble(param.eaReg, 4))
        .WillByDefault(Return(std::to_string(param.eaReg)));

    EXPECT_CALL(*cpu, getAddressingMode(param.eaModeId));
    EXPECT_CALL(*addressingMode, disassemble(param.eaReg, 4));

    ASSERT_EQ(param.expectedDisassembly, subject->disassemble(opWord));
    if (!param.expectTrap) {
        delete addressingResult;
    }
}

INSTANTIATE_TEST_SUITE_P(JSR, JSRTest,
    Values(
        JSRTestParam {
            .testName = "AddressRegisterIndirect",
            .eaModeId = AddressRegisterIndirectMode::MODE_ID,
            .eaReg = 2,
            .eaData = 0xAABBCCDD,
            .initPc = 42,
            .expectedCycles = 16,
            .expectTrap = false,
            .expectedDisassembly = "JSR 2"
        },
        JSRTestParam {
            .testName = "AddressRegisterIndirectDisplacement",
            .eaModeId = AddressRegisterIndirectDisplacementMode::MODE_ID,
            .eaReg = 3,
            .eaData = 0x00001234,
            .initPc = 42,
            .expectedCycles = 18,
            .expectTrap = false,
            .expectedDisassembly = "JSR 3"
        },
        JSRTestParam {
            .testName = "AddressRegisterIndirectWithIndex",
            .eaModeId = AddressRegisterIndirectWithIndexMode::MODE_ID,
            .eaReg = 4,
            .eaData = 0x00001234,
            .initPc = 42,
            .expectedCycles = 22,
            .expectTrap = false,
            .expectedDisassembly = "JSR 4"
        },
        JSRTestParam {
            .testName = "AbsoluteShort",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = AbsoluteShortAddressingMode::MODE_ID,
            .eaData = 0x00001234,
            .initPc = 42,
            .expectedCycles = 18,
            .expectTrap = false,
            .expectedDisassembly = "JSR 0"
        },
        JSRTestParam {
            .testName = "AbsoluteLong",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = AbsoluteLongAddressingMode::MODE_ID,
            .eaData = 0x00001234,
            .initPc = 42,
            .expectedCycles = 20,
            .expectTrap = false,
            .expectedDisassembly = "JSR 1"
        },
        JSRTestParam {
            .testName = "ProgramCounterDisplacement",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = ProgramCounterIndirectDisplacementMode::MODE_ID,
            .eaData = 0x00001234,
            .initPc = 42,
            .expectedCycles = 18,
            .expectTrap = false,
            .expectedDisassembly = "JSR 2"
        },
        JSRTestParam {
            .testName = "ProgramCounterWithIndex",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = ProgramCounterIndirectWithIndexMode::MODE_ID,
            .eaData = 0x00001234,
            .initPc = 42,
            .expectedCycles = 22,
            .expectTrap = false,
            .expectedDisassembly = "JSR 3"
        },
        JSRTestParam {
            .testName = "AddressRegisterDirectIsIllegal",
            .eaModeId = 0b001,
            .eaReg = 2,
            .eaData = 0,
            .initPc = 42,
            .expectedCycles = 34,
            .expectTrap = true,
            .expectedDisassembly = "JSR 2"
        },
        JSRTestParam {
            .testName = "PostIncrementIsIllegal",
            .eaModeId = AddressRegisterIndirectPostIncrementMode::MODE_ID,
            .eaReg = 5,
            .eaData = 0,
            .initPc = 42,
            .expectedCycles = 34,
            .expectTrap = true,
            .expectedDisassembly = "JSR 5"
        },
        JSRTestParam {
            .testName = "ImmediateIsIllegal",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = ImmediateDataMode::MODE_ID,
            .eaData = 0,
            .initPc = 42,
            .expectedCycles = 34,
            .expectTrap = true,
            .expectedDisassembly = "JSR 4"
        }
    ),
    [](const TestParamInfo<JSRTestParam>& info) {
        return info.param.testName;
    }
);
