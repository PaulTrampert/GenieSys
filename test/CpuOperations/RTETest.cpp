//
// Created by paul on 2/28/22.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/RTE.h>
#include "../MockBus.h"
#include "../MockCpu.h"

using namespace GenieSys;

struct RTETestParams {
    bool isSupervisor;
    uint8_t expectedCycles;
};

class RTETest : public testing::TestWithParam<RTETestParams> {
public:
    uint16_t opWord = 0b0100111001110011;
    MockCpu *cpu;
    MockBus *bus;
    RTE *subject;

    RTETest() {
        cpu = new MockCpu();
        bus = new MockBus();
        subject = new RTE(cpu, bus);
    }

    ~RTETest() override {
        delete subject;
        delete bus;
        delete cpu;
    }
};

TEST_P(RTETest, Execute) {
    auto param = GetParam();

    ON_CALL(*cpu, isSupervisor())
        .WillByDefault(testing::Return(param.isSupervisor));
    ON_CALL(*cpu, stackPopWord())
        .WillByDefault(testing::Return(0));
    ON_CALL(*cpu, stackPopLong())
        .WillByDefault(testing::Return(1));
    ON_CALL(*cpu, trap(TV_PRIVILEGE))
        .WillByDefault(testing::Return(34));

    EXPECT_CALL(*cpu, isSupervisor())
        .Times(1);
    EXPECT_CALL(*cpu, setSR(0))
        .Times(param.isSupervisor ? 1 : 0);
    EXPECT_CALL(*cpu, setPc(1))
        .Times(param.isSupervisor ? 1 : 0);
    EXPECT_CALL(*cpu, trap(TV_PRIVILEGE))
        .Times(param.isSupervisor ? 0 : 1);

    ASSERT_EQ(param.expectedCycles, subject->execute(opWord));
}

TEST_P(RTETest, Disassemble) {
    ASSERT_EQ("RTE", subject->disassemble(opWord));
}

INSTANTIATE_TEST_CASE_P(RTE, RTETest,
                        testing::Values(
                                RTETestParams {
                                    .isSupervisor = false,
                                    .expectedCycles = 34
                                },
                                RTETestParams {
                                    .isSupervisor = true,
                                    .expectedCycles = 20
                                }
                                ));