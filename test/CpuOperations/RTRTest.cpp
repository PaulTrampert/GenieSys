//
// Created by paul on 2/28/22.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/RTR.h>
#include "../MockCpu.h"
#include "../MockBus.h"

using namespace GenieSys;

class RTRTest : public testing::Test {
public:
    uint16_t opWord = 0b0100111001110101;
    MockCpu *cpu;
    MockBus *bus;
    RTR *subject;

    RTRTest() {
        cpu = new MockCpu();
        bus = new MockBus();
        subject = new RTR(cpu, bus);
    }

    ~RTRTest() override {
        delete subject;
        delete cpu;
        delete bus;
    }
};

TEST_F(RTRTest, Execute) {
    ON_CALL(*cpu, stackPopWord())
        .WillByDefault((testing::Return(0xFFAA)));
    ON_CALL(*cpu, stackPopLong())
        .WillByDefault(testing::Return(5));

    EXPECT_CALL(*cpu, stackPopWord())
        .Times(1);
    EXPECT_CALL(*cpu, stackPopLong())
        .Times(1);
    EXPECT_CALL(*cpu, setCcrFlags(0xAA))
        .Times(1);
    EXPECT_CALL(*cpu, setPc(5))
        .Times(1);

    ASSERT_EQ(20, subject->execute(opWord));
}

TEST_F(RTRTest, Disassemble) {
    ASSERT_EQ("RTR", subject->disassemble(opWord));
}