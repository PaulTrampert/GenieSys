//
// Created by pault on 9/12/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/CpuOperations/BCLR.h>


struct BCLRTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    Bus bus;
    BCLR* subject;
    uint16_t immModeOpWord = 0b0000100000000001; // BCLR $06,D1
    uint16_t dnModeOpWord = 0b0000011100000001; // BCLR D3,D1

    BCLRTest() {
        cpu = bus.getCpu();
        subject = new BCLR(cpu, &bus);
    }

    ~BCLRTest() override {
        delete subject;
    }

protected:
    void SetUp() override {
        cpu->setPc(20);
        bus.writeByte(21, 6);
    }
};

TEST_F(BCLRTest, SpecificityIsTen) {
    ASSERT_EQ(10, subject->getSpecificity());
}

TEST_F(BCLRTest, DisassembleImmediate) {
    ASSERT_EQ("BCLR $06,D1", subject->disassemble(immModeOpWord));
}

TEST_F(BCLRTest, DisassembleDataRegister) {
    ASSERT_EQ("BCLR D3,D1", subject->disassemble(dnModeOpWord));
}

TEST_F(BCLRTest, ImmMode_SetsCcrZero_WhenSpecifiedBitIsZero) {
    cpu->setDataRegister(1, (uint32_t) 0);

    uint8_t cycles = subject->execute(immModeOpWord);

    ASSERT_EQ(14, cycles);
    ASSERT_EQ(CCR_ZERO, (cpu->getCcrFlags() & CCR_ZERO));
    ASSERT_EQ(0, cpu->getDataRegister(1));
}

TEST_F(BCLRTest, ImmMode_DoesntSetCcrZero_WhenSpecifiedBitIsOne) {
    cpu->setDataRegister(1, (uint32_t) 64);

    uint8_t cycles = subject->execute(immModeOpWord);

    ASSERT_EQ(14, cycles);
    ASSERT_EQ(0, (cpu->getCcrFlags() & CCR_ZERO));
    ASSERT_EQ(0, cpu->getDataRegister(1));
}

TEST_F(BCLRTest, DnMode_SetsCcrZero_WhenSpecifiedBitIsZero) {
    cpu->setDataRegister(1, (uint32_t) 0);
    cpu->setDataRegister(3, (uint32_t) 6);

    uint8_t cycles = subject->execute(dnModeOpWord);

    ASSERT_EQ(10, cycles);
    ASSERT_EQ(CCR_ZERO, (cpu->getCcrFlags() & CCR_ZERO));
    ASSERT_EQ(0, cpu->getDataRegister(1));
}

TEST_F(BCLRTest, DnMode_DoesntSetCcrZero_WhenSpecifiedBitIsOne) {
    cpu->setDataRegister(1, (uint32_t) 64);
    cpu->setDataRegister(3, (uint32_t) 6);

    uint8_t cycles = subject->execute(dnModeOpWord);

    ASSERT_EQ(10, cycles);
    ASSERT_EQ(0, (cpu->getCcrFlags() & CCR_ZERO));
    ASSERT_EQ(0, cpu->getDataRegister(1));
}