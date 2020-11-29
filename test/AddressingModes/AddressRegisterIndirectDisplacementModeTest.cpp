//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h>

struct AddressRegisterIndirectDisplacementModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    AddressRegisterIndirectDisplacementMode* subject;

    AddressRegisterIndirectDisplacementModeTest() {
        cpu = bus.getCpu();
        subject = new AddressRegisterIndirectDisplacementMode(cpu, &bus);
        bus.writeWord(0, -5);
        cpu->setAddressRegister(2, 11);
    }

    ~AddressRegisterIndirectDisplacementModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterIndirectDisplacementModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(6, subject->getAddress(2));
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, ItAdvancesTheProgramCounterOneWord) {
    subject->getAddress(2);
    EXPECT_EQ(2, cpu->getPc());
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, TestGetModeId) {
    EXPECT_EQ(0b101u, subject->getModeId());
}