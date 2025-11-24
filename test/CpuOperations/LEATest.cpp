//
// Created by copilot on 11/24/2024.
//
#include <GenieSys/CpuOperations/LEA.h>
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>

using namespace GenieSys;

class LEATest : public ::testing::Test {
public:
    // LEA (A3),A5 -> 0100 101 111 010 011 = 0x4BD3
    uint16_t opWord = 0b0100101111010011;

    LEA* subject;
    Bus bus;
    M68kCpu* cpu;

    LEATest() : Test() {
        cpu = bus.getCpu();
        subject = new LEA(cpu, &bus);
    }

    ~LEATest() override {
        delete subject;
    }

    void SetUp() override {
        cpu->setAddressRegister(3, 0xAABBCCDD);
        cpu->setAddressRegister(5, 0);
    }
};


TEST_F(LEATest, Disassemble) {
    ASSERT_EQ("LEA (A3),A5", subject->disassemble(opWord));
}

TEST_F(LEATest, Execute) {
    ASSERT_EQ(12, subject->execute(opWord));
    ASSERT_EQ(0xAABBCCDD, cpu->getAddressRegister(5));
}

TEST_F(LEATest, ExecuteDoesNotAffectSourceRegister) {
    subject->execute(opWord);
    ASSERT_EQ(0xAABBCCDD, cpu->getAddressRegister(3));
}
