//
// Created by paul.trampert on 11/30/2020.
//

//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/numberUtils.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h>
#include "GenieSys/TrapException.h"
#include "gmock/gmock-matchers.h"


struct AddressRegisterIndirectPreDecrementModeTest : testing::Test {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;
    GenieSys::AddressRegisterIndirectPreDecrementMode* subject;

    AddressRegisterIndirectPreDecrementModeTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::AddressRegisterIndirectPreDecrementMode(cpu, &bus);
        cpu->setAddressRegister(2, 14);
        bus.writeLong(10, 0x12345678);
    }

    ~AddressRegisterIndirectPreDecrementModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterIndirectPreDecrementModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(14, subject->getAddress(2));
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, ItGetsTheExpectedDataWhenSizeIsOne) {
    const std::vector<uint8_t> &result = subject->getData(2, 1)->getData();
    EXPECT_EQ(0x78, result[0]);
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, ItGetsTheExpectedDataWhenSizeIsTwo) {
    const std::vector<uint8_t> &result = subject->getData(2, 2)->getData();
    EXPECT_EQ(0x5678, GenieSys::bytesToWord(result));
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, ItGetsTheExpectedDataWhenSizeIsFour) {
    const std::vector<uint8_t> &result = subject->getData(2, 4)->getData();
    EXPECT_EQ(0x12345678, GenieSys::bytesToLong(result));
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, ItDecrementsTheAddressRegisterBySizeOne) {
    const std::vector<uint8_t> &result = subject->getData(2, 1)->getData();
    EXPECT_EQ(13, cpu->getAddressRegister(2));
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, ItDecrementsTheAddressRegisterBySizeTwo) {
    const std::vector<uint8_t> &result = subject->getData(2, 2)->getData();
    EXPECT_EQ(12, cpu->getAddressRegister(2));
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, ItDecrementsTheAddressRegisterBySizeFour) {
    const std::vector<uint8_t> &result = subject->getData(2, 4)->getData();
    EXPECT_EQ(10, cpu->getAddressRegister(2));
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, ItDecrementsTheUserStackPointerByMinimumOfTwo) {
    cpu->setAddressRegister(7, 4);
    const std::vector<uint8_t> &result = subject->getData(7, 1)->getData();
    EXPECT_EQ(2, cpu->getAddressRegister(7));
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, TestGetModeId) {
    EXPECT_EQ(0b100u, subject->getModeId());
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, TestDisassemble) {
    EXPECT_EQ("-(A7)", subject->disassemble(7, 0));
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, TestMovemToReg) {
    EXPECT_THROW({
                     try {
                         subject->movemToReg(1, 2, 1);
                     } catch (GenieSys::TrapException &e) {
                         EXPECT_EQ(GenieSys::TV_ILLEGAL_INSTR, e.getTrapVector());
                         throw;
                     }
                 }, GenieSys::TrapException);
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, TestMovemToMemWord) {
    cpu->setAddressRegister(7, 11);
    cpu->setDataRegister(7, 0xFFFFFFF0);
    cpu->setDataRegister(3, (uint32_t)0x4201);
    cpu->setDataRegister(0, (uint32_t)0x5532);
    bus.write(3, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(7, 2, 0b1001000100000001);
    auto written = bus.read(3, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x00, 0x0B}));
    EXPECT_EQ(3, cpu->getAddressRegister(7));
}

TEST_F(AddressRegisterIndirectPreDecrementModeTest, TestMovemToMemLong) {
    cpu->setAddressRegister(2, 11);
    cpu->setAddressRegister(7, 0xFFF04444);
    cpu->setDataRegister(0, (uint32_t)0x55324201);
    bus.write(3, std::vector<uint8_t> {0, 0, 0, 0, 0, 0, 0, 0});

    auto result = subject->movemToMem(2, 4, 0b1000000000000001);
    auto written = bus.read(3, 8);
    EXPECT_THAT(written, testing::ElementsAreArray({0x55, 0x32, 0x42, 0x01, 0xFF, 0xF0, 0x44, 0x44}));
    EXPECT_EQ(3, cpu->getAddressRegister(2));
}