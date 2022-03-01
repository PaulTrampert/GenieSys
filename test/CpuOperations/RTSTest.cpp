//
// Created by paul on 2/28/22.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/RTS.h>
#include "../MockCpu.h"
#include "../MockBus.h"

using namespace GenieSys;

class RTSTest : public testing::Test {
public:
    uint16_t opWord = 0b0100111001110101;
    MockCpu *cpu;
    MockBus *bus;
    RTS *subject;

    RTSTest() {
        cpu = new MockCpu();
        bus = new MockBus();
        subject = new RTS(cpu, bus);
    }

    ~RTSTest() override {
        delete subject;
        delete cpu;
        delete bus;
    }
};

TEST_F(RTSTest, Execute) {
    ON_CALL(*cpu, stackPopLong())
        .WillByDefault(testing::Return(5));

    EXPECT_CALL(*cpu, stackPopLong())
        .Times(1);
    EXPECT_CALL(*cpu, setPc(5))
        .Times(1);

    ASSERT_EQ(16, subject->execute(opWord));
}

TEST_F(RTSTest, Disassemble) {
    ASSERT_EQ("RTS", subject->disassemble(opWord));
}