//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h>
#include "gmock/gmock-matchers.h"


struct AddressRegisterIndirectDisplacementModeTest : testing::Test {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;
    GenieSys::AddressRegisterIndirectDisplacementMode* subject;

    AddressRegisterIndirectDisplacementModeTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::AddressRegisterIndirectDisplacementMode(cpu, &bus);
        bus.writeWord(0, -5);
        cpu->setAddressRegister(2, 11);
        bus.writeWord(6, 0xABCD);
    }

    ~AddressRegisterIndirectDisplacementModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterIndirectDisplacementModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(6, subject->getAddress(2));
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, GettingTheAddressAdvancesTheProgramCounterOneWord) {
    subject->getAddress(2);
    EXPECT_EQ(2, cpu->getPc());
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, ItGetsTheCorrectData) {
    auto result = subject->getData(2, 2)->getData();
    EXPECT_EQ(0xAB, result[0]);
    EXPECT_EQ(0xCD, result[1]);
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, TestGetModeId) {
    EXPECT_EQ(0b101u, subject->getModeId());
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, TestDisassemble) {
    EXPECT_EQ("(-5,A7)", subject->disassemble(7, 0));
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, TestMovemToRegWord) {
    bus.write(6, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(2, 2, 0b1000000010001001);
    EXPECT_EQ(0x5532, cpu->getDataRegister(0));
    EXPECT_EQ(0x4201, cpu->getDataRegister(3));
    EXPECT_EQ(0xFFFFFFF0, cpu->getDataRegister(7));
    EXPECT_EQ(0x4444, cpu->getAddressRegister(7));
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, TestMovemToRegLong) {
    bus.write(6, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(2, 4, 0b1000000000000001);
    EXPECT_EQ(0x55324201, cpu->getDataRegister(0));
    EXPECT_EQ(0, cpu->getDataRegister(3));
    EXPECT_EQ(0, cpu->getDataRegister(7));
    EXPECT_EQ(0xFFF04444, cpu->getAddressRegister(7));
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, TestMovemToMemWord) {
    cpu->setAddressRegister(7, 0x4444);
    cpu->setDataRegister(7, 0xFFFFFFF0);
    cpu->setDataRegister(3, (uint32_t)0x4201);
    cpu->setDataRegister(0, (uint32_t)0x5532);
    bus.write(6, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(2, 2, 0b1000000010001001);
    auto written = bus.read(6, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44}));
}

TEST_F(AddressRegisterIndirectDisplacementModeTest, TestMovemToMemLong) {
    cpu->setAddressRegister(7, 0xFFF04444);
    cpu->setDataRegister(0, (uint32_t)0x55324201);
    bus.write(6, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(2, 4, 0b1000000000000001);
    auto written = bus.read(6, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44}));
}