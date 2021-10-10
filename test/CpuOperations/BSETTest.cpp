//
// Created by pault on 9/12/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/CpuOperations/BSET.h>



struct BSETTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    GenieSys::BSET* subject;
    uint16_t immModeOpWord = 0b0000100011000001; // BSET $06,D1
    uint16_t dnModeOpWord = 0b0000011111000001; // BSET D3,D1

    BSETTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::BSET(cpu, &bus);
    }

    ~BSETTest() override {
        delete subject;
    }

protected:
    void SetUp() override {
        cpu->setPc(20);
        bus.writeByte(21, 6);
    }
};

TEST_F(BSETTest, SpecificityIsTen) {
    ASSERT_EQ(10, subject->getSpecificity());
}

TEST_F(BSETTest, DisassembleImmediate) {
    ASSERT_EQ("BSET $06,D1", subject->disassemble(immModeOpWord));
}

TEST_F(BSETTest, DisassembleDataRegister) {
    ASSERT_EQ("BSET D3,D1", subject->disassemble(dnModeOpWord));
}

TEST_F(BSETTest, ImmMode_SetsCcrZero_WhenSpecifiedBitIsZero) {
    cpu->setDataRegister(1, (uint32_t) 0);

    uint8_t cycles = subject->execute(immModeOpWord);

    ASSERT_EQ(12, cycles);
    ASSERT_EQ(GenieSys::CCR_ZERO, (cpu->getCcrFlags() & GenieSys::CCR_ZERO));
    ASSERT_EQ(64, cpu->getDataRegister(1));
}

TEST_F(BSETTest, ImmMode_DoesntSetCcrZero_WhenSpecifiedBitIsOne) {
    cpu->setDataRegister(1, (uint32_t) 64);

    uint8_t cycles = subject->execute(immModeOpWord);

    ASSERT_EQ(12, cycles);
    ASSERT_EQ(0, (cpu->getCcrFlags() & GenieSys::CCR_ZERO));
    ASSERT_EQ(64, cpu->getDataRegister(1));
}

TEST_F(BSETTest, DnMode_SetsCcrZero_WhenSpecifiedBitIsZero) {
    cpu->setDataRegister(1, (uint32_t) 0);
    cpu->setDataRegister(3, (uint32_t) 6);

    uint8_t cycles = subject->execute(dnModeOpWord);

    ASSERT_EQ(8, cycles);
    ASSERT_EQ(GenieSys::CCR_ZERO, (cpu->getCcrFlags() & GenieSys::CCR_ZERO));
    ASSERT_EQ(64, cpu->getDataRegister(1));
}

TEST_F(BSETTest, DnMode_DoesntSetCcrZero_WhenSpecifiedBitIsOne) {
    cpu->setDataRegister(1, (uint32_t) 64);
    cpu->setDataRegister(3, (uint32_t) 6);

    uint8_t cycles = subject->execute(dnModeOpWord);

    ASSERT_EQ(8, cycles);
    ASSERT_EQ(0, (cpu->getCcrFlags() & GenieSys::CCR_ZERO));
    ASSERT_EQ(64, cpu->getDataRegister(1));
}