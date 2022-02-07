//
// Created by paul.trampert on 11/29/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AbsoluteLongAddressingMode.h>


struct AbsoluteLongAddressingModeTest : testing::Test {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;
    GenieSys::AbsoluteLongAddressingMode* subject;

    AbsoluteLongAddressingModeTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::AbsoluteLongAddressingMode(cpu, &bus);
        cpu->setPc(32);
        bus.writeLong(32, 4096);
        bus.writeByte(4096, 55);
    }

    ~AbsoluteLongAddressingModeTest() override {
        delete subject;
    }
};

TEST_F(AbsoluteLongAddressingModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(4096, subject->getAddress(subject->getModeId()));
}

TEST_F(AbsoluteLongAddressingModeTest, ItAdvancesTheProgramCounterTwoWords) {
    subject->getAddress(subject->getModeId());
    EXPECT_EQ(36, cpu->getPc());
}

TEST_F(AbsoluteLongAddressingModeTest, ItGetsTheCorrectData) {
    auto result = subject->getData(subject->getModeId(), 1);
    EXPECT_EQ(55, result->getData()[0]);
}

TEST_F(AbsoluteLongAddressingModeTest, TestGetModeId) {
    EXPECT_EQ(0b001u, subject->getModeId());
}

TEST_F(AbsoluteLongAddressingModeTest, TestDisassemble) {
    EXPECT_EQ("(4096).L", subject->disassemble(subject->getModeId(), 1));
}