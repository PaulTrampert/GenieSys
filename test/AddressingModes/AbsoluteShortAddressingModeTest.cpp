//
// Created by paul.trampert on 11/29/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AbsoluteShortAddressingMode.h>
#include "gmock/gmock-matchers.h"


struct AbsoluteShortAddressingModeTest : testing::Test {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;
    GenieSys::AbsoluteShortAddressingMode* subject;

    AbsoluteShortAddressingModeTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::AbsoluteShortAddressingMode(cpu, &bus);
        cpu->setPc(32);
        bus.writeWord(32, 9001);
        bus.writeByte(9001, 42);
    }

    ~AbsoluteShortAddressingModeTest() override {
        delete subject;
    }
};

TEST_F(AbsoluteShortAddressingModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(9001, subject->getAddress(subject->getModeId()));
}

TEST_F(AbsoluteShortAddressingModeTest, ItAdvancesTheProgramCounterOneWord) {
    subject->getAddress(subject->getModeId());
    EXPECT_EQ(34, cpu->getPc());
}

TEST_F(AbsoluteShortAddressingModeTest, ItGetsTheCorrectData) {
    auto result = subject->getData(subject->getModeId(), 1);
    EXPECT_EQ(42, result->getData()[0]);
}

TEST_F(AbsoluteShortAddressingModeTest, TestGetModeId) {
    EXPECT_EQ(0b000u, subject->getModeId());
}

TEST_F(AbsoluteShortAddressingModeTest, TestDisassemble) {
    EXPECT_EQ("(9001).W", subject->disassemble(subject->getModeId(), 1));
}

TEST_F(AbsoluteShortAddressingModeTest, TestMovemToRegWord) {
    bus.write(9001, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(GenieSys::AbsoluteShortAddressingMode::MODE_ID, 2, 0b1000000010001001);
    EXPECT_EQ(0x5532, cpu->getDataRegister(0));
    EXPECT_EQ(0x4201, cpu->getDataRegister(3));
    EXPECT_EQ(0xFFFFFFF0, cpu->getDataRegister(7));
    EXPECT_EQ(0x4444, cpu->getAddressRegister(7));
}

TEST_F(AbsoluteShortAddressingModeTest, TestMovemToRegLong) {
    bus.write(9001, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(GenieSys::AbsoluteShortAddressingMode::MODE_ID, 4, 0b1000000000000001);
    EXPECT_EQ(0x55324201, cpu->getDataRegister(0));
    EXPECT_EQ(0, cpu->getDataRegister(3));
    EXPECT_EQ(0, cpu->getDataRegister(7));
    EXPECT_EQ(0xFFF04444, cpu->getAddressRegister(7));
}

TEST_F(AbsoluteShortAddressingModeTest, TestMovemToMemWord) {
    cpu->setAddressRegister(7, 0x4444);
    cpu->setDataRegister(7, 0xFFFFFFF0);
    cpu->setDataRegister(3, (uint32_t)0x4201);
    cpu->setDataRegister(0, (uint32_t)0x5532);
    bus.write(9001, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(GenieSys::AbsoluteShortAddressingMode::MODE_ID, 2, 0b1000000010001001);
    auto written = bus.read(9001, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44}));
}

TEST_F(AbsoluteShortAddressingModeTest, TestMovemToMemLong) {
    cpu->setAddressRegister(7, 0xFFF04444);
    cpu->setDataRegister(0, (uint32_t)0x55324201);
    bus.write(9001, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(GenieSys::AbsoluteShortAddressingMode::MODE_ID, 4, 0b1000000000000001);
    auto written = bus.read(9001, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44}));
}