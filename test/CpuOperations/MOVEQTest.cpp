//
// Created by paul on 5/20/24.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/MOVEQ.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>

using namespace GenieSys;

struct MOVEQTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    MOVEQ* subject;
    uint16_t baseOpWord = 0b0111000000000000;
    uint16_t data = 0b11111111;
    uint16_t dest = 0b111;

    MOVEQTest() {
        cpu = bus.getCpu();
        subject = new MOVEQ(cpu, &bus);
    }

    ~MOVEQTest() override {
        delete subject;
    }
};

TEST_F(MOVEQTest, MovePositiveDataToRegister) {
    data = 0x01;
    subject->execute(baseOpWord | data | (dest << 9));
    ASSERT_EQ(1, cpu->getDataRegister(dest));
}

TEST_F(MOVEQTest, MoveNegativeDataToRegister) {
    data = 0xF0;
    subject->execute(baseOpWord | data | (dest << 9));
    ASSERT_EQ(0xFFFFFFF0, cpu->getDataRegister(dest));
}

TEST_F(MOVEQTest, Disassemble) {
    ASSERT_EQ("MOVEQ #255,D7", subject->disassemble(baseOpWord | data | (dest << 9)));
}