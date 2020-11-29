//
// Created by paul.trampert on 11/29/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/AbsoluteShortAddressingMode.h>

struct AbsoluteShortAddressingModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    AbsoluteShortAddressingMode* subject;

    AbsoluteShortAddressingModeTest() {
        cpu = bus.getCpu();
        subject = new AbsoluteShortAddressingMode(cpu, &bus);
        cpu->setPc(32);
        bus.writeWord(32, 9001);
    }

    ~AbsoluteShortAddressingModeTest() override {
        delete subject;
    }
};

TEST_F(AbsoluteShortAddressingModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(9001, subject->getAddress(subject->getModeId()));
}

TEST_F(AbsoluteShortAddressingModeTest, ItAdvancesTheProgramCounterOneWord) {
    subject->getAddress(subject->getModeId());
    EXPECT_EQ(34, cpu->getPc());
}

TEST_F(AbsoluteShortAddressingModeTest, TestGetModeId) {
    EXPECT_EQ(0b000u, subject->getModeId());
}