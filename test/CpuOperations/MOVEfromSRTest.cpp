//
// Created by pault on 9/26/2021.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/MOVEfromSR.h>


struct MOVEfromSRTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    Bus bus;
    MOVEfromSR* subject;

    uint16_t opWord = 0b0100000011010000;

    MOVEfromSRTest() {
        cpu = bus.getCpu();
        subject = new MOVEfromSR(cpu, &bus);
    }

    ~MOVEfromSRTest() {
        delete subject;
    }

public:
    void SetUp() override {
        cpu->setSR(0x5555);
        cpu->setAddressRegister(0, 666);
    }
};

TEST_F(MOVEfromSRTest, Disassemble) {
    ASSERT_EQ("MOVE SR,(A0)", subject->disassemble(opWord));
}

TEST_F(MOVEfromSRTest, Execute) {
    ASSERT_EQ(12, subject->execute(opWord));
    ASSERT_EQ(0x5555, bus.readWord(666));
}