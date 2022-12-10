//
// Created by paul.trampert on 12/3/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h>
#include <GenieSys/numberUtils.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include "GenieSys/TrapException.h"


struct ProgramCounterIndirectDisplacementModeTest : testing::Test {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;
    GenieSys::ProgramCounterIndirectDisplacementMode* subject;

    ProgramCounterIndirectDisplacementModeTest() {
        cpu = bus.getCpu();
        cpu->setPc(0x1111);
        subject = new GenieSys::ProgramCounterIndirectDisplacementMode(cpu, &bus);
        bus.writeWord(0x1111, 0x1111);
        bus.writeWord(0x2222, 0x3344);
    }

    ~ProgramCounterIndirectDisplacementModeTest() override {
        delete subject;
    }
};

TEST_F(ProgramCounterIndirectDisplacementModeTest, ItGetsTheCorrectMode) {
    ASSERT_EQ(0b010u, subject->getModeId());
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, TheAddressIsTheCpusPcPlusTheDisplacementWord) {
    ASSERT_EQ(0x2222, subject->getAddress(0b010u));
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, GetData_GetsTheCorrectData) {
    ASSERT_EQ(0x3344, GenieSys::bytesToWord(subject->getData(0b010u, 2)->getData()));
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, GetData_IncrementsTheProgramCounter) {
    subject->getData(0b010u, 2);
    ASSERT_EQ(0x1113, cpu->getPc());
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, Disassemble) {
    ASSERT_EQ("(#4369,PC)", subject->disassemble(0b010u, 2));
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, TestMovemToRegWord) {
    bus.write(0x2222, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(GenieSys::ProgramCounterIndirectDisplacementMode::MODE_ID, 2, 0b1000000010001001);
    EXPECT_EQ(0x5532, cpu->getDataRegister(0));
    EXPECT_EQ(0x4201, cpu->getDataRegister(3));
    EXPECT_EQ(0xFFFFFFF0, cpu->getDataRegister(7));
    EXPECT_EQ(0x4444, cpu->getAddressRegister(7));
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, TestMovemToRegLong) {
    bus.write(0x2222, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(GenieSys::ProgramCounterIndirectDisplacementMode::MODE_ID, 4, 0b1000000000000001);
    EXPECT_EQ(0x55324201, cpu->getDataRegister(0));
    EXPECT_EQ(0, cpu->getDataRegister(3));
    EXPECT_EQ(0, cpu->getDataRegister(7));
    EXPECT_EQ(0xFFF04444, cpu->getAddressRegister(7));
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, TestMovemToMem) {
    EXPECT_THROW({
                     try {
                         subject->movemToMem(GenieSys::ProgramCounterIndirectDisplacementMode::MODE_ID, 2, 1);
                     } catch (GenieSys::TrapException &e) {
                         EXPECT_EQ(GenieSys::TV_ILLEGAL_INSTR, e.getTrapVector());
                         throw;
                     }
                 }, GenieSys::TrapException);
}