//
// Created by pault on 9/26/2021.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/MOVEtoSR.h>
#include <GenieSys/M68kCpu.h>

struct MOVEtoSRTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    MOVEtoSR* subject;

    uint16_t opWord = 0b0100010011010000;

    MOVEtoSRTest() {
        cpu = bus.getCpu();
        subject = new MOVEtoSR(cpu, &bus);
    }

    ~MOVEtoSRTest() override {
        delete subject;
    }

public:
    void SetUp() override {
        cpu->setSR(0x0000);
        cpu->setAddressRegister(0, 666);
        bus.writeWord(666, (uint16_t)0xABCD);
    }
};

TEST_F(MOVEtoSRTest, Disassemble) {
    ASSERT_EQ("MOVE (A0),SR", subject->disassemble(opWord));
}

TEST_F(MOVEtoSRTest, ExecuteWhenUnpriveledged) {
    ASSERT_EQ(16, subject->execute(opWord));
    ASSERT_EQ(8192, cpu->getSR());
}

TEST_F(MOVEtoSRTest, ExecuteWhenPriveledged) {
    cpu->setSR(8192);

    ASSERT_EQ(16, subject->execute(opWord));
    ASSERT_EQ(0xABCD, cpu->getSR());
}