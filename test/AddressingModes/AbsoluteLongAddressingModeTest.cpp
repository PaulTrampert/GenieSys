//
// Created by paul.trampert on 11/29/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/AbsoluteLongAddressingMode.h>

struct AbsoluteLongAddressingModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    AbsoluteLongAddressingMode* subject;

    AbsoluteLongAddressingModeTest() {
        cpu = bus.getCpu();
        subject = new AbsoluteLongAddressingMode(cpu, &bus);
        cpu->setPc(32);
        bus.writeLong(32, 9876543);
    }

    ~AbsoluteLongAddressingModeTest() override {
        delete subject;
    }
};

TEST_F(AbsoluteLongAddressingModeTest, TestGetAddress) {
    EXPECT_EQ(9876543, subject->getAddress(subject->getModeId()));
}

TEST_F(AbsoluteLongAddressingModeTest, TestGetModeId) {
    EXPECT_EQ(0b001u, subject->getModeId());
}