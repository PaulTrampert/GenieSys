//
// Created by paul.trampert on 11/30/2020.
//

//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/numberUtils.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectPreDecrementMode.h>


struct AddressRegisterIndirectPreDecrementModeTest : testing::Test {
    Bus bus;
    GenieSys::M68kCpu* cpu;
    AddressRegisterIndirectPreDecrementMode* subject;

    AddressRegisterIndirectPreDecrementModeTest() {
        cpu = bus.getCpu();
        subject = new AddressRegisterIndirectPreDecrementMode(cpu, &bus);
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