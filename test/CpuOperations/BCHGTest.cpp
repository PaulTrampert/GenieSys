//
// Created by paul.trampert on 4/4/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/BCHG.h>


struct BCHGTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    Bus bus;
    BCHG* subject;

    BCHGTest() {
        cpu = bus.getCpu();
        subject = new BCHG(cpu, &bus);
    }

    ~BCHGTest() override {
        delete subject;
    }
};


TEST_F(BCHGTest, SpecificityIsTen) {
    ASSERT_EQ(10, subject->getSpecificity());
}

TEST_F(BCHGTest, DisassembleImmediate) {
    cpu->setPc(20);
    bus.writeWord(20, 0x0003);

    ASSERT_EQ("BCHG $03,D7", subject->disassemble(0b0000100000000111));
}

TEST_F(BCHGTest, DisassembleDataRegister) {
    ASSERT_EQ("BCHG D2,D7", subject->disassemble(0b0000010100000111));
}

TEST_F(BCHGTest, ImmModeSetsZeroFlag_WhenSpecifiedBitIsZero) {
    cpu->setDataRegister(0, (uint32_t)0x00000000);
    cpu->setCcrFlags(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW);
    cpu->setPc(20);
    bus.writeWord(20, 0x0003);

    uint8_t cycles = subject->execute(0b0000100000000000);

    ASSERT_EQ(12, cycles);
    ASSERT_EQ((CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW | CCR_ZERO), cpu->getCcrFlags());
    ASSERT_EQ(8, cpu->getDataRegister(0));
}

TEST_F(BCHGTest, ImmModeDoesNotSetZeroFlag_WhenSpecifiedBitIsNotZero) {
    cpu->setDataRegister(0, (uint32_t)0b00000000000000000000000000001000);
    cpu->setCcrFlags(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW);
    cpu->setPc(20);
    bus.writeWord(20, 0x0003);

    uint8_t cycles = subject->execute(0b0000100000000000);

    ASSERT_EQ(12, cycles);
    ASSERT_EQ((CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW), cpu->getCcrFlags());
    ASSERT_EQ(0, cpu->getDataRegister(0));
}

TEST_F(BCHGTest, RegModeSetsZeroFlag_WhenSpecifiedBitIsZero) {
    cpu->setDataRegister(0, (uint32_t)0x00000000);
    cpu->setCcrFlags(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW);
    cpu->setDataRegister(2, (uint32_t)35);

    uint8_t cycles = subject->execute(0b0000010100000000);

    ASSERT_EQ(8, cycles);
    ASSERT_EQ((CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW | CCR_ZERO), cpu->getCcrFlags());
    ASSERT_EQ(8, cpu->getDataRegister(0));
}

TEST_F(BCHGTest, RegModeDoesNotSetZeroFlag_WhenSpecifiedBitIsNotZero) {
    cpu->setDataRegister(0, (uint32_t)0b00000000000000000000000000001000);
    cpu->setCcrFlags(CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW);
    cpu->setDataRegister(2, (uint32_t)35);

    uint8_t cycles = subject->execute(0b0000010100000000);

    ASSERT_EQ(8, cycles);
    ASSERT_EQ((CCR_CARRY | CCR_EXTEND | CCR_NEGATIVE | CCR_OVERFLOW), cpu->getCcrFlags());
    ASSERT_EQ(0, cpu->getDataRegister(0));
}