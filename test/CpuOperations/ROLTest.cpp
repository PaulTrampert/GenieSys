//
// Created by copilot on implementation of ROL tests
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ROL.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>

class ROLTest : public ::testing::Test {
public:
    GenieSys::ROL* subject;
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;

    // Register rotate immediate byte: ROL.b #1, D0 = 0b1110 001 1 00 0 11 000 = 0xE318
    uint16_t byteImmOp = 0xE318;
    // Register rotate immediate word: ROL.w #1, D0 = 0b1110 001 1 01 0 11 000 = 0xE358
    uint16_t wordImmOp = 0xE358;
    // Register rotate immediate long: ROL.l #1, D0 = 0b1110 001 1 10 0 11 000 = 0xE398
    uint16_t longImmOp = 0xE398;
    // Register rotate register byte: ROL.b D1, D0 = 0b1110 001 1 00 1 11 000 = 0xE338
    uint16_t byteRegOp = 0xE338;
    // Memory rotate: ROL.w (A0) = 0b1110 011 1 11 010 000 = 0xE7D0
    uint16_t memOp = 0xE7D0;

    ROLTest() : Test() {
        cpu = bus.getCpu();
        subject = new GenieSys::ROL(cpu, &bus);
    }

    ~ROLTest() override {
        delete subject;
    }
};

TEST_F(ROLTest, DisassembleByteImmediate) {
    ASSERT_EQ("ROL.b #1, D0", subject->disassemble(byteImmOp));
}

TEST_F(ROLTest, DisassembleWordImmediate) {
    ASSERT_EQ("ROL.w #1, D0", subject->disassemble(wordImmOp));
}

TEST_F(ROLTest, DisassembleLongImmediate) {
    ASSERT_EQ("ROL.l #1, D0", subject->disassemble(longImmOp));
}

TEST_F(ROLTest, DisassembleByteRegister) {
    ASSERT_EQ("ROL.b D1, D0", subject->disassemble(byteRegOp));
}

TEST_F(ROLTest, DisassembleMemory) {
    ASSERT_EQ("ROL.w (A0)", subject->disassemble(memOp));
}

TEST_F(ROLTest, ExecuteByteImmediateRotate1) {
    // Rotate 0x80 left by 1: bit 7 goes to bit 0 and carry
    cpu->setDataRegister(0, (uint8_t)0x80);
    subject->execute(byteImmOp);
    ASSERT_EQ(0x01, cpu->getDataRegister(0) & 0xFF);
    ASSERT_EQ(GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROLTest, ExecuteByteImmediateRotate1NoCarry) {
    // Rotate 0x40 left by 1: no carry
    cpu->setDataRegister(0, (uint8_t)0x40);
    subject->execute(byteImmOp);
    ASSERT_EQ(0x80, cpu->getDataRegister(0) & 0xFF);
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(ROLTest, ExecuteByteImmediateRotate8) {
    // ROL.b #8, D0 - count 0 means 8 rotations
    // 0b1110 000 1 00 0 11 000 = 0xE118
    uint16_t op = 0xE118;
    cpu->setDataRegister(0, (uint8_t)0x81);
    subject->execute(op);
    // After 8 rotations, 0x81 should become 0x81 again
    ASSERT_EQ(0x81, cpu->getDataRegister(0) & 0xFF);
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROLTest, ExecuteWordImmediate) {
    cpu->setDataRegister(0, (uint16_t)0x8000);
    subject->execute(wordImmOp);
    ASSERT_EQ(0x0001, cpu->getDataRegister(0) & 0xFFFF);
    ASSERT_EQ(GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROLTest, ExecuteLongImmediate) {
    cpu->setDataRegister(0, (uint32_t)0x80000000);
    subject->execute(longImmOp);
    ASSERT_EQ(0x00000001u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROLTest, ExecuteRegisterMode) {
    // ROL.b D1, D0 where D1 contains the count
    cpu->setDataRegister(1, (uint32_t)4);  // Rotate by 4
    cpu->setDataRegister(0, (uint8_t)0x12);  // 0001 0010 -> 0010 0001 = 0x21
    subject->execute(byteRegOp);
    ASSERT_EQ(0x21, cpu->getDataRegister(0) & 0xFF);
}

TEST_F(ROLTest, ExecuteRegisterModeZeroCount) {
    // When count is 0, no rotation happens and carry is cleared
    cpu->setDataRegister(1, (uint32_t)0);
    cpu->setDataRegister(0, (uint8_t)0x80);
    cpu->setCcrFlags(GenieSys::CCR_CARRY);  // Set carry initially
    subject->execute(byteRegOp);
    ASSERT_EQ(0x80, cpu->getDataRegister(0) & 0xFF);
    // Carry should be cleared when count is 0
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(ROLTest, ExecuteRegisterModeMod64) {
    // Register mode: count modulo 64
    cpu->setDataRegister(1, (uint32_t)65);  // 65 mod 64 = 1
    cpu->setDataRegister(0, (uint8_t)0x80);
    subject->execute(byteRegOp);
    ASSERT_EQ(0x01, cpu->getDataRegister(0) & 0xFF);
}

TEST_F(ROLTest, ExecuteMemoryRotate) {
    bus.writeWord(600, 0x8000);
    cpu->setAddressRegister(0, 600);
    subject->execute(memOp);
    ASSERT_EQ(0x0001, bus.readWord(600));
    ASSERT_EQ(GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROLTest, ExecuteZeroResultSetsZeroFlag) {
    cpu->setDataRegister(0, (uint8_t)0x00);
    subject->execute(byteImmOp);
    ASSERT_EQ(0x00, cpu->getDataRegister(0) & 0xFF);
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(ROLTest, ExecuteNegativeResultSetsNegativeFlag) {
    cpu->setDataRegister(0, (uint8_t)0x40);  // 0100 0000 -> 1000 0000 after rotate left by 1
    subject->execute(byteImmOp);
    ASSERT_EQ(0x80, cpu->getDataRegister(0) & 0xFF);
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(ROLTest, ExtendFlagNotAffected) {
    cpu->setCcrFlags(GenieSys::CCR_EXTEND);
    cpu->setDataRegister(0, (uint8_t)0x80);
    subject->execute(byteImmOp);
    // Extend flag should still be set
    ASSERT_TRUE(cpu->getCcrFlags() & GenieSys::CCR_EXTEND);
}

TEST_F(ROLTest, OverflowAlwaysCleared) {
    cpu->setCcrFlags(GenieSys::CCR_OVERFLOW);
    cpu->setDataRegister(0, (uint8_t)0x80);
    subject->execute(byteImmOp);
    // Overflow should be cleared
    ASSERT_FALSE(cpu->getCcrFlags() & GenieSys::CCR_OVERFLOW);
}
