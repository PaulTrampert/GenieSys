//
// Created by pault on 10/5/2021.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/CLR.h>



class CLRTest : public ::testing::Test {
public:
    CLR* subject;
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;

    uint16_t byteOp = 0b0100001000010000;
    uint16_t wordOp = 0b0100001001010000;
    uint16_t longOp = 0b0100001010010000;


    CLRTest() : Test() {
        cpu = bus.getCpu();
        subject = new CLR(cpu, &bus);
    }

    ~CLRTest() override {
        delete subject;
    }

protected:
    void SetUp() override {
        bus.writeLong(600, 0xAABBCCDD);
        cpu->setAddressRegister(0, 600);
    }
};

TEST_F(CLRTest, DisassembleByte) {
    ASSERT_EQ("CLR.b (A0)", subject->disassemble(byteOp));
}

TEST_F(CLRTest, DisassembleWord) {
    ASSERT_EQ("CLR.w (A0)", subject->disassemble(wordOp));
}

TEST_F(CLRTest, DisassembleLong) {
    ASSERT_EQ("CLR.l (A0)", subject->disassemble(longOp));
}

TEST_F(CLRTest, ExecuteByte) {
    ASSERT_EQ(12, subject->execute(byteOp));
    ASSERT_EQ(0x00BBCCDD, bus.readLong(600));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(CLRTest, ExecuteWord) {
    ASSERT_EQ(12, subject->execute(wordOp));
    ASSERT_EQ(0x0000CCDD, bus.readLong(600));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(CLRTest, ExecuteLong) {
    ASSERT_EQ(20, subject->execute(longOp));
    ASSERT_EQ(0x00000000, bus.readLong(600));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}