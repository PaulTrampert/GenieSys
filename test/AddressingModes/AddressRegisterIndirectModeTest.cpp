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
        bus.writeWord(11, 0xABCD);
    }

    ~AddressRegisterIndirectModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterIndirectModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(11, subject->getAddress(2));
}

TEST_F(AddressRegisterIndirectModeTest, ItGetsTheCorrectData) {
    auto result = subject->getData(2, 2);
    EXPECT_EQ(0xAB, result[0]);
    EXPECT_EQ(0xCD, result[1]);
}

TEST_F(AddressRegisterIndirectModeTest, TestGetModeId) {
    EXPECT_EQ(0b010u, subject->getModeId());
}