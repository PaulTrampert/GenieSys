//
// Created by pault on 10/5/2021.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/EXT.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>


class EXTTest : public ::testing::Test {
public:
    GenieSys::EXT* subject;
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;

    uint16_t wordOp = 0b0100100010000000;
    uint16_t longOp = 0b0100100011000000;


    EXTTest() : Test() {
        cpu = bus.getCpu();
        subject = new GenieSys::EXT(cpu, &bus);
    }

    ~EXTTest() override {
        delete subject;
    }

protected:
};

TEST_F(EXTTest, DisassembleWord) {
    ASSERT_EQ("EXT.w D0", subject->disassemble(wordOp));
}

TEST_F(EXTTest, DisassembleLong) {
    ASSERT_EQ("EXT.l D0", subject->disassemble(longOp));
}

TEST_F(EXTTest, ExecuteWordNonNegative) {
    cpu->setDataRegister(0, 0xFFFFFF00);
    ASSERT_EQ(4, subject->execute(wordOp));
    ASSERT_EQ(0xFFFF0000, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(EXTTest, ExecuteWordNegative) {
    cpu->setDataRegister(0, (uint32_t)0x000000FF);
    ASSERT_EQ(4, subject->execute(wordOp));
    ASSERT_EQ(0x0000FFFF, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(EXTTest, ExecuteLongNonNegative) {
    cpu->setDataRegister(0, 0xFFFF0000);
    ASSERT_EQ(4, subject->execute(longOp));
    ASSERT_EQ(0, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(EXTTest, ExecuteLongNegative) {
    cpu->setDataRegister(0, (uint32_t)0x0000FFFF);
    ASSERT_EQ(4, subject->execute(longOp));
    ASSERT_EQ(0xFFFFFFFF, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}