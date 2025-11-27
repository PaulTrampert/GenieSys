//
// Created by copilot on implementation of ROR tests
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ROR.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>

class RORTest : public ::testing::Test {
public:
    GenieSys::ROR* subject;
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;

    // Register rotate immediate byte: ROR.b #1, D0 = 0b1110 001 0 00 0 11 000 = 0xE218
    uint16_t byteImmOp = 0xE218;
    // Register rotate immediate word: ROR.w #1, D0 = 0b1110 001 0 01 0 11 000 = 0xE258
    uint16_t wordImmOp = 0xE258;
    // Register rotate immediate long: ROR.l #1, D0 = 0b1110 001 0 10 0 11 000 = 0xE298
    uint16_t longImmOp = 0xE298;
    // Register rotate register byte: ROR.b D1, D0 = 0b1110 001 0 00 1 11 000 = 0xE238
    uint16_t byteRegOp = 0xE238;
    // Memory rotate: ROR.w (A0) = 0b1110 011 0 11 010 000 = 0xE6D0
    uint16_t memOp = 0xE6D0;

    RORTest() : Test() {
        cpu = bus.getCpu();
        subject = new GenieSys::ROR(cpu, &bus);
    }

    ~RORTest() override {
        delete subject;
    }
};

TEST_F(RORTest, DisassembleByteImmediate) {
    ASSERT_EQ("ROR.b #1, D0", subject->disassemble(byteImmOp));
}

TEST_F(RORTest, DisassembleWordImmediate) {
    ASSERT_EQ("ROR.w #1, D0", subject->disassemble(wordImmOp));
}

TEST_F(RORTest, DisassembleLongImmediate) {
    ASSERT_EQ("ROR.l #1, D0", subject->disassemble(longImmOp));
}

TEST_F(RORTest, DisassembleByteRegister) {
    ASSERT_EQ("ROR.b D1, D0", subject->disassemble(byteRegOp));
}

TEST_F(RORTest, DisassembleMemory) {
    ASSERT_EQ("ROR.w (A0)", subject->disassemble(memOp));
}

TEST_F(RORTest, ExecuteByteImmediateRotate1) {
    // Rotate 0x01 right by 1: bit 0 goes to bit 7 and carry
    cpu->setDataRegister(0, (uint8_t)0x01);
    subject->execute(byteImmOp);
    ASSERT_EQ(0x80, cpu->getDataRegister(0) & 0xFF);
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(RORTest, ExecuteByteImmediateRotate1NoCarry) {
    // Rotate 0x02 right by 1: no carry
    cpu->setDataRegister(0, (uint8_t)0x02);
    subject->execute(byteImmOp);
    ASSERT_EQ(0x01, cpu->getDataRegister(0) & 0xFF);
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(RORTest, ExecuteByteImmediateRotate8) {
    // ROR.b #8, D0 - count 0 means 8 rotations
    // 0b1110 000 0 00 0 11 000 = 0xE018
    uint16_t op = 0xE018;
    cpu->setDataRegister(0, (uint8_t)0x81);
    subject->execute(op);
    // After 8 rotations, 0x81 should become 0x81 again
    ASSERT_EQ(0x81, cpu->getDataRegister(0) & 0xFF);
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(RORTest, ExecuteWordImmediate) {
    cpu->setDataRegister(0, (uint16_t)0x0001);
    subject->execute(wordImmOp);
    ASSERT_EQ(0x8000, cpu->getDataRegister(0) & 0xFFFF);
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(RORTest, ExecuteLongImmediate) {
    cpu->setDataRegister(0, (uint32_t)0x00000001);
    subject->execute(longImmOp);
    ASSERT_EQ(0x80000000u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(RORTest, ExecuteRegisterMode) {
    // ROR.b D1, D0 where D1 contains the count
    cpu->setDataRegister(1, (uint32_t)4);  // Rotate by 4
    cpu->setDataRegister(0, (uint8_t)0x12);  // 0001 0010 -> 0010 0001 = 0x21
    subject->execute(byteRegOp);
    ASSERT_EQ(0x21, cpu->getDataRegister(0) & 0xFF);
}

TEST_F(RORTest, ExecuteRegisterModeZeroCount) {
    // When count is 0, no rotation happens and carry is cleared
    cpu->setDataRegister(1, (uint32_t)0);
    cpu->setDataRegister(0, (uint8_t)0x01);
    cpu->setCcrFlags(GenieSys::CCR_CARRY);  // Set carry initially
    subject->execute(byteRegOp);
    ASSERT_EQ(0x01, cpu->getDataRegister(0) & 0xFF);
    // Carry should be cleared when count is 0
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(RORTest, ExecuteRegisterModeMod64) {
    // Register mode: count modulo 64
    cpu->setDataRegister(1, (uint32_t)65);  // 65 mod 64 = 1
    cpu->setDataRegister(0, (uint8_t)0x01);
    subject->execute(byteRegOp);
    ASSERT_EQ(0x80, cpu->getDataRegister(0) & 0xFF);
}

TEST_F(RORTest, ExecuteMemoryRotate) {
    bus.writeWord(600, 0x0001);
    cpu->setAddressRegister(0, 600);
    subject->execute(memOp);
    ASSERT_EQ(0x8000, bus.readWord(600));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(RORTest, ExecuteZeroResultSetsZeroFlag) {
    cpu->setDataRegister(0, (uint8_t)0x00);
    subject->execute(byteImmOp);
    ASSERT_EQ(0x00, cpu->getDataRegister(0) & 0xFF);
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(RORTest, ExecuteNegativeResultSetsNegativeFlag) {
    cpu->setDataRegister(0, (uint8_t)0x01);  // 0000 0001 -> 1000 0000 after rotate right by 1
    subject->execute(byteImmOp);
    ASSERT_EQ(0x80, cpu->getDataRegister(0) & 0xFF);
    ASSERT_TRUE(cpu->getCcrFlags() & GenieSys::CCR_NEGATIVE);
}

TEST_F(RORTest, ExtendFlagNotAffected) {
    cpu->setCcrFlags(GenieSys::CCR_EXTEND);
    cpu->setDataRegister(0, (uint8_t)0x01);
    subject->execute(byteImmOp);
    // Extend flag should still be set
    ASSERT_TRUE(cpu->getCcrFlags() & GenieSys::CCR_EXTEND);
}

TEST_F(RORTest, OverflowAlwaysCleared) {
    cpu->setCcrFlags(GenieSys::CCR_OVERFLOW);
    cpu->setDataRegister(0, (uint8_t)0x01);
    subject->execute(byteImmOp);
    // Overflow should be cleared
    ASSERT_FALSE(cpu->getCcrFlags() & GenieSys::CCR_OVERFLOW);
}
