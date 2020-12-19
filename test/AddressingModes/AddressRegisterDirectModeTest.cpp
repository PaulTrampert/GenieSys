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
        cpu->setAddressRegister(3, 9999);
        subject = new AddressRegisterDirectMode(cpu, &bus);
    }

    ~AddressRegisterDirectModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterDirectModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(3, subject->getAddress(3));
}

TEST_F(AddressRegisterDirectModeTest, ItGetsTheCorrectData) {
    std::vector<uint8_t> result = subject->getData(3, 4)->getData();
    uint32_t finalResult = 0;
    for (int i = 0; i < 4; i++) {
        finalResult |= result[i] << (8 * (3 - i));
    }
    EXPECT_EQ(9999, finalResult);
}

TEST_F(AddressRegisterDirectModeTest, TestGetModeId) {
    EXPECT_EQ(0b001u, subject->getModeId());
}

TEST_F(AddressRegisterDirectModeTest, TestDisassemble) {
    EXPECT_EQ("A7", subject->disassemble(7, 0));
}