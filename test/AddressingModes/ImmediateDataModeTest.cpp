//
// Created by paul.trampert on 12/3/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/numberUtils.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include "GenieSys/TrapException.h"


struct ImmediateDataModeTest : testing::Test {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;
    GenieSys::ImmediateDataMode* subject;

    ImmediateDataModeTest() {
        cpu = bus.getCpu();
        cpu->setPc(32);
        subject = new GenieSys::ImmediateDataMode(cpu, &bus);
        bus.writeWord(32, 0x1111);
        bus.writeWord(34, 0x0a00);
    }

    ~ImmediateDataModeTest() override {
        delete subject;
    }
};

TEST_F(ImmediateDataModeTest, ItGetsTheCorrectMode) {
    ASSERT_EQ(0b100u, subject->getModeId());
}

TEST_F(ImmediateDataModeTest, TheAddressIsTheCpusPc) {
    ASSERT_EQ(32, subject->getAddress(0b100u));
}

TEST_F(ImmediateDataModeTest, GetData_GetsTheCorrectData) {
    ASSERT_EQ(0x1111, GenieSys::bytesToWord(subject->getData(0b100u, 2)->getData()));
}

TEST_F(ImmediateDataModeTest, GetData_IncrementsTheProgramCounter) {
    subject->getData(0b100u, 2);
    ASSERT_EQ(34, cpu->getPc());
}

TEST_F(ImmediateDataModeTest, Disassemble_ReturnsTheCorrectString) {
    ASSERT_EQ("$1111", subject->disassemble(0b100, 2));
}

TEST_F(ImmediateDataModeTest, Disassemble_ReturnsTheCorrectStringWithZeros) {
    cpu->setPc(34);
    ASSERT_EQ("$0a00", subject->disassemble(0b100, 2));
}

TEST_F(ImmediateDataModeTest, TestMovemToReg) {
    EXPECT_THROW({
                     try {
                         subject->movemToReg(1, 2, 1);
                     } catch (GenieSys::TrapException &e) {
                         EXPECT_EQ(GenieSys::TV_ILLEGAL_INSTR, e.getTrapVector());
                         throw;
                     }
                 }, GenieSys::TrapException);
}