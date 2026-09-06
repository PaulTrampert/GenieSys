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
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include "../MockCpu.h"
#include "../MockBus.h"
#include "../MockAddressingMode.h"

using namespace GenieSys;
using namespace testing;

struct JSRTestParam {
    std::string testName;
    uint8_t eaModeId;
    uint8_t eaReg;
    uint32_t eaAddress;
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
    JSR *subject;

    JSRTest() : TestWithParam() {
        cpu = new MockCpu();
        bus = new MockBus();
        addressingMode = new MockAddressingMode();
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
    if (param.expectTrap) {
        ON_CALL(*cpu, trap(TV_ILLEGAL_INSTR)).WillByDefault(Return(param.expectedCycles));
        EXPECT_CALL(*cpu, trap(TV_ILLEGAL_INSTR));
        EXPECT_CALL(*cpu, getAddressingMode(_)).Times(0);
        EXPECT_CALL(*cpu, stackPushLong(_)).Times(0);
        EXPECT_CALL(*cpu, setPc(_)).Times(0);
    }
    else {
        ON_CALL(*cpu, getAddressingMode(param.eaModeId))
            .WillByDefault(Return(addressingMode));
        ON_CALL(*cpu, getPc())
            .WillByDefault(Return(param.initPc));
        ON_CALL(*addressingMode, getAddress(param.eaReg))
            .WillByDefault(Return(param.eaAddress));

        EXPECT_CALL(*cpu, getAddressingMode(param.eaModeId));
        EXPECT_CALL(*addressingMode, getAddress(param.eaReg));
        // The destination is the effective address itself, so the operand is never read.
        EXPECT_CALL(*addressingMode, getDataProxy(_, _)).Times(0);
        EXPECT_CALL(*cpu, getPc());
        EXPECT_CALL(*cpu, stackPushLong(param.initPc));
        EXPECT_CALL(*cpu, setPc(param.eaAddress));
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
}

// The mocked tests above pin the contract; this one pins the behaviour end to end, because a
// JSR that read its operand would land on the contents of the address rather than the address.
TEST(JSRWithRealCpuTest, JumpsToTheEffectiveAddressAndNotItsContents) {
    Bus bus;
    M68kCpu* cpu = bus.getCpu();
    JSR subject(cpu, &bus);
    cpu->setPc(0x100);
    cpu->setAddressRegister(0, 0x400);
    cpu->setStackPointer(0x800);
    bus.writeLong(0x400, 0x1234);

    // JSR (A0) -> 0100 1110 10 010 000
    ASSERT_EQ(16, subject.execute(0b0100111010010000));
    ASSERT_EQ(0x400, cpu->getPc());
    ASSERT_EQ(0x100, bus.readLong(cpu->getStackPointer()));
}

INSTANTIATE_TEST_SUITE_P(JSR, JSRTest,
    Values(
        JSRTestParam {
            .testName = "AddressRegisterIndirect",
            .eaModeId = AddressRegisterIndirectMode::MODE_ID,
            .eaReg = 2,
            .eaAddress = 0xAABBCCDD,
            .initPc = 42,
            .expectedCycles = 16,
            .expectTrap = false,
            .expectedDisassembly = "JSR 2"
        },
        JSRTestParam {
            .testName = "AddressRegisterIndirectDisplacement",
            .eaModeId = AddressRegisterIndirectDisplacementMode::MODE_ID,
            .eaReg = 3,
            .eaAddress = 0x00001234,
            .initPc = 42,
            .expectedCycles = 18,
            .expectTrap = false,
            .expectedDisassembly = "JSR 3"
        },
        JSRTestParam {
            .testName = "AddressRegisterIndirectWithIndex",
            .eaModeId = AddressRegisterIndirectWithIndexMode::MODE_ID,
            .eaReg = 4,
            .eaAddress = 0x00001234,
            .initPc = 42,
            .expectedCycles = 22,
            .expectTrap = false,
            .expectedDisassembly = "JSR 4"
        },
        JSRTestParam {
            .testName = "AbsoluteShort",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = AbsoluteShortAddressingMode::MODE_ID,
            .eaAddress = 0x00001234,
            .initPc = 42,
            .expectedCycles = 18,
            .expectTrap = false,
            .expectedDisassembly = "JSR 0"
        },
        JSRTestParam {
            .testName = "AbsoluteLong",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = AbsoluteLongAddressingMode::MODE_ID,
            .eaAddress = 0x00001234,
            .initPc = 42,
            .expectedCycles = 20,
            .expectTrap = false,
            .expectedDisassembly = "JSR 1"
        },
        JSRTestParam {
            .testName = "ProgramCounterDisplacement",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = ProgramCounterIndirectDisplacementMode::MODE_ID,
            .eaAddress = 0x00001234,
            .initPc = 42,
            .expectedCycles = 18,
            .expectTrap = false,
            .expectedDisassembly = "JSR 2"
        },
        JSRTestParam {
            .testName = "ProgramCounterWithIndex",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = ProgramCounterIndirectWithIndexMode::MODE_ID,
            .eaAddress = 0x00001234,
            .initPc = 42,
            .expectedCycles = 22,
            .expectTrap = false,
            .expectedDisassembly = "JSR 3"
        },
        JSRTestParam {
            .testName = "AddressRegisterDirectIsIllegal",
            .eaModeId = 0b001,
            .eaReg = 2,
            .eaAddress = 0,
            .initPc = 42,
            .expectedCycles = 34,
            .expectTrap = true,
            .expectedDisassembly = "JSR 2"
        },
        JSRTestParam {
            .testName = "PostIncrementIsIllegal",
            .eaModeId = AddressRegisterIndirectPostIncrementMode::MODE_ID,
            .eaReg = 5,
            .eaAddress = 0,
            .initPc = 42,
            .expectedCycles = 34,
            .expectTrap = true,
            .expectedDisassembly = "JSR 5"
        },
        JSRTestParam {
            .testName = "ImmediateIsIllegal",
            .eaModeId = ProgramCounterAddressingMode::MODE_ID,
            .eaReg = ImmediateDataMode::MODE_ID,
            .eaAddress = 0,
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
