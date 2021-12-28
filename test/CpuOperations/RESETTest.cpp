//
// Created by pault on 12/28/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/RESET.h>
#include "../MockBus.h"
#include "../MockCpu.h"

using namespace GenieSys;

struct RESETTest : testing::Test {
    uint16_t opWord = 0b0100111001110000;
    MockBus bus;
    MockCpu cpu;
    RESET * subject;

    RESETTest() : Test() {
        subject = new RESET(&cpu, &bus);
    }

    ~RESETTest() override {
        delete subject;
    }
};

TEST_F(RESETTest, ExecuteCallsResetOnBus) {
    EXPECT_CALL(bus, reset()).Times(1);

    uint8_t cycles = subject->execute(opWord);

    ASSERT_EQ(124, cycles);
}