//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPostIncrementMode.h>

struct AddressRegisterIndirectPostIncrementModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    AddressRegisterIndirectPostIncrementMode* subject;

    AddressRegisterIndirectPostIncrementModeTest() {
        cpu = bus.getCpu();
        subject = new AddressRegisterIndirectPostIncrementMode(cpu, &bus);
        cpu->setAddressRegister(2, 11);
    }

    ~AddressRegisterIndirectPostIncrementModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterIndirectPostIncrementModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(11, subject->getAddress(2));
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, TestGetModeId) {
    EXPECT_EQ(0b011u, subject->getModeId());
}