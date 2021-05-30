//
// Created by paul.trampert on 4/4/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/BTST.h>

struct BTSTTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    BTST* subject;

    BTSTTest() {
        cpu = bus.getCpu();
        subject = new BTST(cpu, &bus);
    }

    ~BTSTTest() override {
        delete subject;
    }
};


TEST_F(BTSTTest, SpecificityIsTen) {
    ASSERT_EQ(10, subject->getSpecificity());
}

TEST_F(BTSTTest, DisassembleImmediate) {
    cpu->setPc(20);
    bus.writeWord(20, 0x0003);

    ASSERT_EQ("BTST $03,D7", subject->disassemble(0b0000100000000111));
}

TEST_F(BTSTTest, DisassembleDataRegister) {
    ASSERT_EQ("BTST D2,D7", subject->disassemble(0b0000010100000111));
}

TEST_F(BTSTTest, ImmModeSetsZeroFlag_WhenSpecifiedBitIsZero) {
    cpu->setDataRegister(0, (uint32_t)0x00000000);
    cpu->setCcrFlags(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW);
    cpu->setPc(20);
    bus.writeWord(20, 0x0003);

    uint8_t cycles = subject->execute(0b0000100000000000);

    ASSERT_EQ(10, cycles);
    ASSERT_EQ((CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW | CCR_ZERO), cpu->getCcrFlags());
}

TEST_F(BTSTTest, ImmModeDoesNotSetZeroFlag_WhenSpecifiedBitIsNotZero) {
    cpu->setDataRegister(0, (uint32_t)0b00000000000000000000000000001000);
    cpu->setCcrFlags(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW);
    cpu->setPc(20);
    bus.writeWord(20, 0x0003);

    uint8_t cycles = subject->execute(0b0000100000000000);

    ASSERT_EQ(10, cycles);
    ASSERT_EQ((CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW), cpu->getCcrFlags());
}

TEST_F(BTSTTest, RegModeSetsZeroFlag_WhenSpecifiedBitIsZero) {
    cpu->setDataRegister(0, (uint32_t)0x00000000);
    cpu->setCcrFlags(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW);
    cpu->setDataRegister(2, (uint32_t)35);

    uint8_t cycles = subject->execute(0b0000010100000000);

    ASSERT_EQ(6, cycles);
    ASSERT_EQ((CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW | CCR_ZERO), cpu->getCcrFlags());
}

TEST_F(BTSTTest, RegModeDoesNotSetZeroFlag_WhenSpecifiedBitIsNotZero) {
    cpu->setDataRegister(0, (uint32_t)0b00000000000000000000000000001000);
    cpu->setCcrFlags(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW);
    cpu->setDataRegister(2, (uint32_t)35);

    uint8_t cycles = subject->execute(0b0000010100000000);

    ASSERT_EQ(6, cycles);
    ASSERT_EQ((CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW), cpu->getCcrFlags());
}