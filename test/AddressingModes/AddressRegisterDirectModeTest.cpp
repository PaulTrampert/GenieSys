//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>

struct AddressRegisterDirectModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    AddressRegisterDirectMode* subject;

    AddressRegisterDirectModeTest() {
        cpu = bus.getCpu();
        subject = new AddressRegisterDirectMode(cpu, &bus);
    }

    ~AddressRegisterDirectModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterDirectModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(3, subject->getAddress(3));
}

TEST_F(AddressRegisterDirectModeTest, TestGetModeId) {
    EXPECT_EQ(0b001u, subject->getModeId());
}