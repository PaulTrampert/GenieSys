//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPostIncrementMode.h>
#include <GenieSys/numberUtils.h>

struct AddressRegisterIndirectPostIncrementModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    AddressRegisterIndirectPostIncrementMode* subject;

    AddressRegisterIndirectPostIncrementModeTest() {
        cpu = bus.getCpu();
        subject = new AddressRegisterIndirectPostIncrementMode(cpu, &bus);
        cpu->setAddressRegister(2, 10);
        bus.writeLong(10, 0x12345678);
    }

    ~AddressRegisterIndirectPostIncrementModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterIndirectPostIncrementModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(10, subject->getAddress(2));
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, ItGetsTheExpectedDataWhenSizeIsOne) {
    const std::vector<uint8_t> &result = subject->getData(2, 1)->getData();
    EXPECT_EQ(0x12, result[0]);
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, ItGetsTheExpectedDataWhenSizeIsTwo) {
    const std::vector<uint8_t> &result = subject->getData(2, 2)->getData();
    EXPECT_EQ(0x1234, bytesToWord(result));
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, ItGetsTheExpectedDataWhenSizeIsFour) {
    const std::vector<uint8_t> &result = subject->getData(2, 4)->getData();
    EXPECT_EQ(0x12345678, bytesToLong(result));
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, ItIncrementsTheAddressRegisterBySizeOne) {
    const std::vector<uint8_t> &result = subject->getData(2, 1)->getData();
    EXPECT_EQ(11, cpu->getAddressRegister(2));
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, ItIncrementsTheAddressRegisterBySizeTwo) {
    const std::vector<uint8_t> &result = subject->getData(2, 2)->getData();
    EXPECT_EQ(12, cpu->getAddressRegister(2));
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, ItIncrementsTheAddressRegisterBySizeFour) {
    const std::vector<uint8_t> &result = subject->getData(2, 4)->getData();
    EXPECT_EQ(14, cpu->getAddressRegister(2));
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, ItIncrementsTheUserStackPointerByMinimumOfTwo) {
    const std::vector<uint8_t> &result = subject->getData(7, 1)->getData();
    EXPECT_EQ(2, cpu->getAddressRegister(7));
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, TestGetModeId) {
    EXPECT_EQ(0b011u, subject->getModeId());
}

TEST_F(AddressRegisterIndirectPostIncrementModeTest, TestDisassemble) {
    EXPECT_EQ("(A7)+", subject->disassemble(7, 0));
}