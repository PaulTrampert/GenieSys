//
// Created by pault on 10/5/2021.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/NEG.h>


class NEGTest : public ::testing::Test {
public:
    NEG* subject;
    M68kCpu* cpu;
    Bus bus;

    uint16_t byteOp = 0b0100010000010000;
    uint16_t wordOp = 0b0100010001010000;
    uint16_t longOp = 0b0100010010010000;


    NEGTest() : Test() {
        cpu = bus.getCpu();
        subject = new NEG(cpu, &bus);
    }

    ~NEGTest() override {
        delete subject;
    }

protected:
    void SetUp() override {
        cpu->setAddressRegister(0, 600);
    }
};

TEST_F(NEGTest, DisassembleByte) {
    ASSERT_EQ("NEG.b (A0)", subject->disassemble(byteOp));
}

TEST_F(NEGTest, DisassembleWord) {
    ASSERT_EQ("NEG.w (A0)", subject->disassemble(wordOp));
}

TEST_F(NEGTest, DisassembleLong) {
    ASSERT_EQ("NEG.l (A0)", subject->disassemble(longOp));
}

TEST_F(NEGTest, ExecuteByte) {
    bus.writeByte(600, 100);

    ASSERT_EQ(12, subject->execute(byteOp));
    ASSERT_EQ(-100, (int8_t)bus.read(600));
    ASSERT_EQ(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(NEGTest, ExecuteWord) {
    bus.writeWord(600, 100);

    ASSERT_EQ(12, subject->execute(wordOp));
    ASSERT_EQ(-100, (int16_t)bus.readWord(600));
    ASSERT_EQ(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(NEGTest, ExecuteLong) {
    bus.writeLong(600, 100);

    ASSERT_EQ(20, subject->execute(longOp));
    ASSERT_EQ(-100, (int32_t)bus.readLong(600));
    ASSERT_EQ(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE, cpu->getCcrFlags());
}