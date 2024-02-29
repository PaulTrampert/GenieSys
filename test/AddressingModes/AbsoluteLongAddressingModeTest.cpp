//
// Created by paul.trampert on 11/29/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AbsoluteLongAddressingMode.h>
#include "gmock/gmock-matchers.h"


struct AbsoluteLongAddressingModeTest : testing::Test {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;
    GenieSys::AbsoluteLongAddressingMode* subject;

    AbsoluteLongAddressingModeTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::AbsoluteLongAddressingMode(cpu, &bus);
        cpu->setPc(32);
        bus.writeLong(32, 4096);
        bus.writeByte(4096, 55);
    }

    ~AbsoluteLongAddressingModeTest() override {
        delete subject;
    }
};

TEST_F(AbsoluteLongAddressingModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(4096, subject->getAddress(subject->getModeId()));
}

TEST_F(AbsoluteLongAddressingModeTest, ItAdvancesTheProgramCounterTwoWords) {
    subject->getAddress(subject->getModeId());
    EXPECT_EQ(36, cpu->getPc());
}

TEST_F(AbsoluteLongAddressingModeTest, ItGetsTheCorrectData) {
    auto result = subject->getData(subject->getModeId(), 1);
    EXPECT_EQ(55, result->getData()[0]);
}

TEST_F(AbsoluteLongAddressingModeTest, TestGetModeId) {
    EXPECT_EQ(0b001u, subject->getModeId());
}

TEST_F(AbsoluteLongAddressingModeTest, TestDisassemble) {
    EXPECT_EQ("(4096).L", subject->disassemble(subject->getModeId(), 1));
}

TEST_F(AbsoluteLongAddressingModeTest, TestMovemToRegWord) {
    bus.write(4096, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(GenieSys::AbsoluteLongAddressingMode::MODE_ID, 2, 0b1000000010001001);
    EXPECT_EQ(0x5532, cpu->getDataRegister(0));
    EXPECT_EQ(0x4201, cpu->getDataRegister(3));
    EXPECT_EQ(0xFFFFFFF0, cpu->getDataRegister(7));
    EXPECT_EQ(0x4444, cpu->getAddressRegister(7));
}

TEST_F(AbsoluteLongAddressingModeTest, TestMovemToRegLong) {
    bus.write(4096, std::vector<uint8_t> {0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44});
    auto result = subject->movemToReg(GenieSys::AbsoluteLongAddressingMode::MODE_ID, 4, 0b1000000000000001);
    EXPECT_EQ(0x55324201, cpu->getDataRegister(0));
    EXPECT_EQ(0, cpu->getDataRegister(3));
    EXPECT_EQ(0, cpu->getDataRegister(7));
    EXPECT_EQ(0xFFF04444, cpu->getAddressRegister(7));
}

TEST_F(AbsoluteLongAddressingModeTest, TestMovemToMemWord) {
    cpu->setAddressRegister(7, 0x4444);
    cpu->setDataRegister(7, 0xFFFFFFF0);
    cpu->setDataRegister(3, (uint32_t)0x4201);
    cpu->setDataRegister(0, (uint32_t)0x5532);
    bus.write(4096, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(GenieSys::AbsoluteLongAddressingMode::MODE_ID, 2, 0b1000000010001001);
    auto written = bus.read(4096, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44}));
}

TEST_F(AbsoluteLongAddressingModeTest, TestMovemToMemLong) {
    cpu->setAddressRegister(7, 0xFFF04444);
    cpu->setDataRegister(0, (uint32_t)0x55324201);
    bus.write(4096, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(GenieSys::AbsoluteLongAddressingMode::MODE_ID, 4, 0b1000000000000001);
    auto written = bus.read(4096, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44}));
}