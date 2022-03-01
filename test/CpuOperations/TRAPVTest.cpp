#include <GenieSys/CpuOperations/TRAPV.h>
#include <gtest/gtest.h>
#include <GenieSys/enums.h>
#include "../MockBus.h"
#include "../MockCpu.h"

using namespace GenieSys;

struct TRAPVTestParams {
    bool isOverflow;
    uint8_t expectedCycles;
};

class TRAPVTest : public ::testing::TestWithParam<TRAPVTestParams> {
public:
    TRAPV* subject;
    MockBus *bus;
    MockCpu *cpu;

    uint16_t baseOpWord = 0b0100111001110110;

    TRAPVTest() : TestWithParam() {
        bus = new MockBus();
        cpu = new MockCpu();
        subject = new TRAPV(cpu, bus);
    }

    ~TRAPVTest() override {
        delete subject;
        delete bus;
        delete cpu;
    }
};

TEST_P(TRAPVTest, Execute) {
    TRAPVTestParams params = GetParam();
    ON_CALL(*cpu, getCcrFlags())
        .WillByDefault(testing::Return(params.isOverflow ? CCR_OVERFLOW : 0));
    ON_CALL(*cpu, trap(TV_TRAPV))
        .WillByDefault(testing::Return(34));

    EXPECT_CALL(*cpu, trap(TV_TRAPV))
        .Times(params.isOverflow ? 1 : 0);

    ASSERT_EQ(params.expectedCycles, subject->execute(baseOpWord));
}

TEST_P(TRAPVTest, Disassemble) {
    ASSERT_EQ("TRAPV", subject->disassemble(baseOpWord));
}

INSTANTIATE_TEST_SUITE_P(TRAPV, TRAPVTest,
    testing::Values(
        TRAPVTestParams {
            .isOverflow = true,
            .expectedCycles = 34
        },
        TRAPVTestParams {
            .isOverflow = false,
            .expectedCycles = 4
        }
    )
);