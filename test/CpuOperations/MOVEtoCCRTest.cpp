//
// Created by pault on 9/26/2021.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/MOVEtoCCR.h>

struct MOVEtoCCRTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    MOVEtoCCR* subject;

    uint16_t opWord = 0b0100010011010000;

    MOVEtoCCRTest() {
        cpu = bus.getCpu();
        subject = new MOVEtoCCR(cpu, &bus);
    }

    ~MOVEtoCCRTest() override {
        delete subject;
    }

public:
    void SetUp() override {
        cpu->setSR(0x0000);
        cpu->setAddressRegister(0, 666);
        bus.writeWord(666, (uint16_t)0xABCD);
    }
};

TEST_F(MOVEtoCCRTest, Disassemble) {
    ASSERT_EQ("MOVE (A0),CCR", subject->disassemble(opWord));
}

TEST_F(MOVEtoCCRTest, Execute) {
    ASSERT_EQ(16, subject->execute(opWord));
    ASSERT_EQ(0x00CD, cpu->getSR());
}