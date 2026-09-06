//
// Created by paul on 3/17/22.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/JMP.h>
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

struct JMPTestParam {
    std::string testName;
    uint8_t eaModeId;
    uint8_t eaReg;
    uint32_t eaData;
    // JMP is documented with a complete execution time per addressing mode rather than a base
    // plus an effective address calculation, so the addressing mode's own cycles never apply.
    uint8_t expectedCycles;
    bool expectTrap;
    std::string expectedDisassembly;
};

class JMPTest : public TestWithParam<JMPTestParam> {
public:
    uint16_t opWordBase = 0b0100111011000000;
    BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5,3);
    BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    MockCpu *cpu;
    MockBus *bus;
    MockAddressingMode *addressingMode;
    MockAddressingResult *addressingResult;
    JMP *subject;

    JMPTest() : TestWithParam() {
        cpu = new MockCpu();
        bus = new MockBus();
        addressingMode = new MockAddressingMode();
        addressingResult = new MockAddressingResult();
        subject = new JMP(cpu, bus);
    }

    ~JMPTest() override {
        delete subject;
        delete cpu;
        delete bus;
        delete addressingMode;
    }
};

TEST_P(JMPTest, Execute) {
    auto param = GetParam();
    uint16_t opWord = opWordBase;
    opWord = eaModeMask.compose(opWord, param.eaModeId);
    opWord = eaRegMask.compose(opWord, param.eaReg);
    Mock::AllowLeak(addressingResult);

    if (param.expectTrap) {
        ON_CALL(*cpu, trap(TV_ILLEGAL_INSTR)).WillByDefault(Return(param.expectedCycles));
        EXPECT_CALL(*cpu, trap(TV_ILLEGAL_INSTR));
        EXPECT_CALL(*cpu, getAddressingMode(_)).Times(0);
        EXPECT_CALL(*cpu, setPc(_)).Times(0);
        delete addressingResult;
    }
    else {
        ON_CALL(*cpu, getAddressingMode(param.eaModeId))
            .WillByDefault(Return(addressingMode));
        ON_CALL(*addressingMode, getDataProxy(param.eaReg, 4))
            .WillByDefault(Return(addressingResult));
        ON_CALL(*addressingResult, getDataAsLong())
            .WillByDefault(Return(param.eaData));

        EXPECT_CALL(*cpu, getAddressingMode(param.eaModeId));
        EXPECT_CALL(*addressingMode, getDataProxy(param.eaReg, 4));
        EXPECT_CALL(*addressingResult, getDataAsLong());
        EXPECT_CALL(*cpu, setPc(param.eaData));
        // The addressing mode's cycle count is not part of a JMP's execution time.
        EXPECT_CALL(*addressingResult, getCycles()).Times(0);
    }

    ASSERT_EQ(param.expectedCycles, subject->execute(opWord));
}

TEST_P(JMPTest, Disassemble) {
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

INSTANTIATE_TEST_SUITE_P(JMP, JMPTest,
    Values(
        JMPTestParam {
            .testName = "AddressRegisterIndirect",
            .eaModeId = AddressRegisterIndirectMode::MODE_ID,
            .eaReg = 2,
            .eaData = 0xAABBCCDD,
            .expectedCycles = 8,
            .expectTrap = false,
            .expectedDisassembly = "JMP 2"
        },
        JMPTestParam {
            .testName = "AddressRegisterIndirectDisplacement",
            .eaModeId = AddressRegisterIndirectDisplacementMode::MODE_ID,
            .eaReg = 3,
            .eaData = 0x00001234,
            .expectedCycles = 10,
            .expectTrap = false,
            .expectedDisassembly = "JMP 3"
        },
        JMPTestParam {
            .testName = "AddressRegisterIndirectWithIndex",
            .eaModeId = AddressRegisterIndirectWithIndexMode::MODE_ID,
            .eaReg = 4,
            .eaData = 0x00001234,
            .expectedCycles = 14,
            .expectTrap = false,
            .expectedDisassembly = "JMP 4"
        },
        JMPTestParam {
            .testName = "AbsoluteShort",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = AbsoluteShortAddressingMode::MODE_ID,
            .eaData = 0x00001234,
            .expectedCycles = 10,
            .expectTrap = false,
            .expectedDisassembly = "JMP 0"
        },
        JMPTestParam {
            .testName = "AbsoluteLong",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = AbsoluteLongAddressingMode::MODE_ID,
            .eaData = 0x00001234,
            .expectedCycles = 12,
            .expectTrap = false,
            .expectedDisassembly = "JMP 1"
        },
        JMPTestParam {
            .testName = "ProgramCounterDisplacement",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = ProgramCounterIndirectDisplacementMode::MODE_ID,
            .eaData = 0x00001234,
            .expectedCycles = 10,
            .expectTrap = false,
            .expectedDisassembly = "JMP 2"
        },
        JMPTestParam {
            .testName = "ProgramCounterWithIndex",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = ProgramCounterIndirectWithIndexMode::MODE_ID,
            .eaData = 0x00001234,
            .expectedCycles = 14,
            .expectTrap = false,
            .expectedDisassembly = "JMP 3"
        },
        JMPTestParam {
            .testName = "AddressRegisterDirectIsIllegal",
            .eaModeId = 0b001,
            .eaReg = 2,
            .eaData = 0,
            .expectedCycles = 34,
            .expectTrap = true,
            .expectedDisassembly = "JMP 2"
        },
        JMPTestParam {
            .testName = "PostIncrementIsIllegal",
            .eaModeId = AddressRegisterIndirectPostIncrementMode::MODE_ID,
            .eaReg = 5,
            .eaData = 0,
            .expectedCycles = 34,
            .expectTrap = true,
            .expectedDisassembly = "JMP 5"
        },
        JMPTestParam {
            .testName = "ImmediateIsIllegal",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = ImmediateDataMode::MODE_ID,
            .eaData = 0,
            .expectedCycles = 34,
            .expectTrap = true,
            .expectedDisassembly = "JMP 4"
        }
    ),
    [](const TestParamInfo<JMPTestParam>& info) {
        return info.param.testName;
    }
);
