//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectMode.h>

struct AddressRegisterIndirectModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    AddressRegisterIndirectMode* subject;

    AddressRegisterIndirectModeTest() {
        cpu = bus.getCpu();
        subject = new AddressRegisterIndirectMode(cpu, &bus);
        bus.writeWord(0, -5);
        cpu->setAddressRegister(2, 11);
    }

    ~AddressRegisterIndirectModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterIndirectModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(11, subject->getAddress(2));
}

TEST_F(AddressRegisterIndirectModeTest, TestGetModeId) {
    EXPECT_EQ(0b010u, subject->getModeId());
}