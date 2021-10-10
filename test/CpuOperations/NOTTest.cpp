//
// Created by pault on 10/5/2021.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/NOT.h>
#include <GenieSys/Bus.h>


class NOTTest : public ::testing::Test {
public:
    NOT* subject;
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;

    uint16_t byteOp = 0b0100011000000000;
    uint16_t wordOp = 0b0100011001000000;
    uint16_t longOp = 0b0100011010000000;


    NOTTest() : Test() {
        cpu = bus.getCpu();
        subject = new NOT(cpu, &bus);
    }

    ~NOTTest() override {
        delete subject;
    }

protected:
    void SetUp() override {
        cpu->setDataRegister(0, 0xFFFFFFFF);
    }
};

TEST_F(NOTTest, DisassembleByte) {
    ASSERT_EQ("NOT.b D0", subject->disassemble(byteOp));
}

TEST_F(NOTTest, DisassembleWord) {
    ASSERT_EQ("NOT.w D0", subject->disassemble(wordOp));
}

TEST_F(NOTTest, DisassembleLong) {
    ASSERT_EQ("NOT.l D0", subject->disassemble(longOp));
}

TEST_F(NOTTest, ExecuteByte) {
    ASSERT_EQ(4, subject->execute(byteOp));
    ASSERT_EQ(0xFFFFFF00, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(NOTTest, ExecuteWord) {
    ASSERT_EQ(4, subject->execute(wordOp));
    ASSERT_EQ(0xFFFF0000, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(NOTTest, ExecuteLong) {
    ASSERT_EQ(6, subject->execute(longOp));
    ASSERT_EQ(0, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}