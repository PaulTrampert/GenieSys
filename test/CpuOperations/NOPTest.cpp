//
// Created by paul on 9/6/25.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/NOP.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>

using namespace GenieSys;

struct NOPTest : testing::Test {
    uint16_t opWord = 0b0100111001110001;
    Bus bus;
    M68kCpu* cpu;
    NOP* subject;

    NOPTest() {
        cpu = bus.getCpu();
        subject = new NOP(cpu, &bus);
    }

    ~NOPTest() override {
        delete subject;
    }
};

TEST_F(NOPTest, Disassemble) {
    ASSERT_EQ("NOP", subject->disassemble(opWord));
}

TEST_F(NOPTest, ExecuteTakes4Cycles) {
    ASSERT_EQ(4, subject->execute(opWord));
}
