//
// Created by pault on 10/3/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/CpuOperations/NEGX.h>

struct NEGXTest : public ::testing::Test {
    uint16_t byteOpWord = 0b0100000000000001u;
    uint16_t wordOpWord = 0b0100000001000001u;
    uint16_t longOpWord = 0b0100000010000001u;
    M68kCpu* cpu;
    Bus bus;
    NEGX* subject;

    NEGXTest() {
        cpu = bus.getCpu();
        subject = new NEGX(cpu, &bus);
    }

    ~NEGXTest() override {
        delete subject;
    }
};

TEST_F(NEGXTest, DisassembleByteOp) {
    ASSERT_EQ("NEGX.b D1", subject->disassemble(byteOpWord));
}

TEST_F(NEGXTest, DisassembleWordOp) {
    ASSERT_EQ("NEGX.w D1", subject->disassemble(wordOpWord));
}

TEST_F(NEGXTest, DisassembleLongOp) {
    ASSERT_EQ("NEGX.l D1", subject->disassemble(longOpWord));
}

TEST_F(NEGXTest, ExecuteByteOpNonZero) {
    cpu->setDataRegister(1, (uint8_t)1);

    ASSERT_EQ(4, subject->execute(byteOpWord));
    ASSERT_EQ(-1, (int8_t)cpu->getDataRegister(1));
    ASSERT_EQ(CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(NEGXTest, ExecuteWordOpNonZero) {
    cpu->setDataRegister(1, (uint16_t)1);

    ASSERT_EQ(4, subject->execute(wordOpWord));
    ASSERT_EQ(-1, (int16_t)cpu->getDataRegister(1));
    ASSERT_EQ(CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(NEGXTest, ExecuteLongOpNonZero) {
    cpu->setDataRegister(1, (uint32_t)1);

    ASSERT_EQ(6, subject->execute(longOpWord));
    ASSERT_EQ(-1, (int32_t)cpu->getDataRegister(1));
    ASSERT_EQ(CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(NEGXTest, ExecuteByteOpZero) {
    cpu->setDataRegister(1, (uint8_t)0);

    ASSERT_EQ(4, subject->execute(byteOpWord));
    ASSERT_EQ(0, (int8_t)cpu->getDataRegister(1));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(NEGXTest, ExecuteWordOpZero) {
    cpu->setDataRegister(1, (uint16_t)0);

    ASSERT_EQ(4, subject->execute(wordOpWord));
    ASSERT_EQ(0, (int16_t)cpu->getDataRegister(1));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(NEGXTest, ExecuteLongOpZero) {
    cpu->setDataRegister(1, (uint32_t)0);

    ASSERT_EQ(6, subject->execute(longOpWord));
    ASSERT_EQ(0, (int32_t)cpu->getDataRegister(1));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(NEGXTest, ClearsCcrZeroWhenResultNotZero) {
    cpu->setDataRegister(1, (uint32_t)5);
    cpu->setCcrFlags(CCR_ZERO);

    subject->execute(longOpWord);

    ASSERT_EQ(CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(NEGXTest, DoesNotSetCcrZeroWhenResultIsZero) {
    cpu->setDataRegister(1, (uint32_t)0);

    subject->execute(longOpWord);

    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(NEGXTest, ExtendBitIsUsed) {
    int32_t data = -5;
    cpu->setDataRegister(1, (uint32_t)data);
    cpu->setCcrFlags(CCR_EXTEND);

    subject->execute(longOpWord);

    ASSERT_EQ(4, (int32_t)cpu->getDataRegister(1));
}