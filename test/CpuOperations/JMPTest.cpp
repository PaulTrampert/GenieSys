//
// Created by paul on 3/17/22.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/JMP.h>
#include <GenieSys/BitMask.h>
#include "../MockCpu.h"
#include "../MockBus.h"
#include "../MockAddressingMode.h"

using namespace GenieSys;
using namespace testing;

struct JMPTestParam {
    uint8_t eaModeId;
    uint8_t eaReg;
    uint8_t eaCycles;
    uint32_t eaData;
    uint32_t initPc;
    uint8_t expectedCycles;
    std::string expectedDisassembly;
};

class JMPTest : public TestWithParam<JMPTestParam> {
public:
    uint16_t opWordBase = 0b0100111010000000;
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
    ON_CALL(*cpu, getAddressingMode(param.eaModeId))
        .WillByDefault(Return(addressingMode));
    ON_CALL(*addressingMode, getDataProxy(param.eaReg, 4))
        .WillByDefault(Return(addressingResult));
    ON_CALL(*addressingResult, getDataAsLong())
        .WillByDefault(Return(param.eaData));
    ON_CALL(*addressingResult, getCycles())
        .WillByDefault(Return(param.eaCycles));
    Mock::AllowLeak(addressingResult);

    EXPECT_CALL(*cpu, getAddressingMode(param.eaModeId));
    EXPECT_CALL(*addressingMode, getDataProxy(param.eaReg, 4));
    EXPECT_CALL(*addressingResult, getDataAsLong());
    EXPECT_CALL(*cpu, setPc(param.eaData));
    EXPECT_CALL(*addressingResult, getCycles());

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
    delete addressingResult;
}

INSTANTIATE_TEST_CASE_P(JMP, JMPTest,
                        Values(
                                JMPTestParam {
                                        .eaModeId = 1,
                                        .eaReg = 2,
                                        .eaCycles = 1,
                                        .eaData = 0xAABBCCDD,
                                        .initPc = 42,
                                        .expectedCycles = 9,
                                        .expectedDisassembly = "JMP 2"
                                }
                                ));