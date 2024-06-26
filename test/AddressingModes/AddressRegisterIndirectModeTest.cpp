//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectMode.h>
#include "gmock/gmock-matchers.h"


struct AddressRegisterIndirectModeTest : testing::Test {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;
    GenieSys::AddressRegisterIndirectMode* subject;

    AddressRegisterIndirectModeTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::AddressRegisterIndirectMode(cpu, &bus);
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
    auto result = subject->getData(2, 2)->getData();
    EXPECT_EQ(0xAB, result[0]);
    EXPECT_EQ(0xCD, result[1]);
}

TEST_F(AddressRegisterIndirectModeTest, TestGetModeId) {
    EXPECT_EQ(0b010u, subject->getModeId());
}

TEST_F(AddressRegisterIndirectModeTest, TestDisassemble) {
    EXPECT_EQ("(A7)", subject->disassemble(7, 0));
}

TEST_F(AddressRegisterIndirectModeTest, TestMovemToRegWord) {
    bus.write(11, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(2, 2, 0b1000000010001001);
    EXPECT_EQ(0x5532, cpu->getDataRegister(0));
    EXPECT_EQ(0x4201, cpu->getDataRegister(3));
    EXPECT_EQ(0xFFFFFFF0, cpu->getDataRegister(7));
    EXPECT_EQ(0x4444, cpu->getAddressRegister(7));
}

TEST_F(AddressRegisterIndirectModeTest, TestMovemToRegLong) {
    bus.write(11, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(2, 4, 0b1000000000000001);
    EXPECT_EQ(0x55324201, cpu->getDataRegister(0));
    EXPECT_EQ(0, cpu->getDataRegister(3));
    EXPECT_EQ(0, cpu->getDataRegister(7));
    EXPECT_EQ(0xFFF04444, cpu->getAddressRegister(7));
}

TEST_F(AddressRegisterIndirectModeTest, TestMovemToMemWord) {
    cpu->setAddressRegister(7, 0x4444);
    cpu->setDataRegister(7, 0xFFFFFFF0);
    cpu->setDataRegister(3, (uint32_t)0x4201);
    cpu->setDataRegister(0, (uint32_t)0x5532);
    bus.write(11, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(2, 2, 0b1000000010001001);
    auto written = bus.read(11, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44}));
}

TEST_F(AddressRegisterIndirectModeTest, TestMovemToMemLong) {
    cpu->setAddressRegister(7, 0xFFF04444);
    cpu->setDataRegister(0, (uint32_t)0x55324201);
    bus.write(11, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(2, 4, 0b1000000000000001);
    auto written = bus.read(11, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44}));
}