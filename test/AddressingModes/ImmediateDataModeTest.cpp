//
// Created by paul.trampert on 12/3/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/AddressingModes/ImmediateDataMode.h>
#include <GenieSys/numberUtils.h>

struct ImmediateDataModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    ImmediateDataMode* subject;

    ImmediateDataModeTest() {
        cpu = bus.getCpu();
        cpu->setPc(32);
        subject = new ImmediateDataMode(cpu, &bus);
        bus.writeWord(32, 0x1111);
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
    ASSERT_EQ(0x1111, bytesToWord(subject->getData(0b100u, 2)));
}

TEST_F(ImmediateDataModeTest, GetData_IncrementsTheProgramCounter) {
    subject->getData(0b100u, 2);
    ASSERT_EQ(34, cpu->getPc());
}