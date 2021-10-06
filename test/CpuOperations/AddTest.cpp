//
// Created by paul.trampert on 12/6/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/Add.h>

struct AddTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    Add* subject;

    AddTest() {
        cpu = bus.getCpu();
        subject = new Add(cpu, &bus);
    }

    ~AddTest() override {
        delete subject;
    }
};

TEST_F(AddTest, ItAddsTwoBytes_EffectiveAddressResult) {
    cpu->setDataRegister(0, (uint8_t)55);
    cpu->setDataRegister(1, (uint8_t)20);

    subject->execute(0b1101001100000000);

    ASSERT_EQ(75, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(AddTest, ItAddsTwoWords_EffectiveAddressResult) {
    cpu->setDataRegister(0, (uint16_t)55);
    cpu->setDataRegister(1, (uint16_t)20);

    subject->execute(0b1101001101000000);

    ASSERT_EQ(75, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(AddTest, ItAddsTwoLongs_EffectiveAddressResult) {
    cpu->setDataRegister(0, (uint32_t)55);
    cpu->setDataRegister(1, (uint32_t)20);

    subject->execute(0b1101001110000000);

    ASSERT_EQ(75, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(AddTest, ItDisassembles_EffectiveAddressResult) {
    ASSERT_EQ("ADD.b D1,D0", subject->disassemble(0b1101001100000000));
}

TEST_F(AddTest, ItAddsTwoBytes_DataRegResult) {
    cpu->setDataRegister(0, (uint8_t)55);
    cpu->setDataRegister(1, (uint8_t)20);

    subject->execute(0b1101001000000000);

    ASSERT_EQ(75, cpu->getDataRegister(1));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(AddTest, ItAddsTwoWords_DataRegResult) {
    cpu->setDataRegister(0, (uint16_t)55);
    cpu->setDataRegister(1, (uint16_t)20);

    subject->execute(0b1101001001000000);

    ASSERT_EQ(75, cpu->getDataRegister(1));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(AddTest, ItAddsTwoLongs_DataRegResult) {
    cpu->setDataRegister(0, (uint32_t)55);
    cpu->setDataRegister(1, (uint32_t)20);

    subject->execute(0b1101001010000000);

    ASSERT_EQ(75, cpu->getDataRegister(1));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(AddTest, ItDisassembles_DataRegResult) {
    ASSERT_EQ("ADD.b D0,D1", subject->disassemble(0b1101001000000000));
}

TEST_F(AddTest, ItAddsTwoBytes_WhenOneIsNegative) {
    cpu->setDataRegister(0, (uint8_t)55);
    cpu->setDataRegister(1, (uint8_t)-20);

    subject->execute(0b1101001000000000);

    ASSERT_EQ(35, cpu->getDataRegister(1));
    ASSERT_EQ(CCR_EXTEND | CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(AddTest, ItCorrectlySetsOverflow) {
    cpu->setDataRegister(0, (uint8_t)127);
    cpu->setDataRegister(1, (uint8_t)1);

    subject->execute(0b1101001000000000);

    ASSERT_EQ(128, cpu->getDataRegister(1));
    ASSERT_EQ(CCR_OVERFLOW | CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(AddTest, ItReturnsTheCorrectCycles_AddBytes_EffectiveAddressResult) {
    ASSERT_EQ(8, subject->execute(0b1101001100000000));
}

TEST_F(AddTest, ItReturnsTheCorrectCycles_AddWords_EffectiveAddressResult) {
    ASSERT_EQ(8, subject->execute(0b1101001101000000));
}

TEST_F(AddTest, ItReturnsTheCorrectCycles_AddLongs_EffectiveAddressResult) {
    ASSERT_EQ(12, subject->execute(0b1101001110000000));
}

TEST_F(AddTest, ItReturnsTheCorrectCycles_AddBytes_DataRegisterResult) {
    ASSERT_EQ(4, subject->execute(0b1101001000000000));
}

TEST_F(AddTest, ItReturnsTheCorrectCycles_AddWords_DataRegisterResult) {
    ASSERT_EQ(4, subject->execute(0b1101001001000000));
}

TEST_F(AddTest, ItReturnsTheCorrectCycles_AddLongs_DataRegisterResult_DataRegisterSource) {
    ASSERT_EQ(8, subject->execute(0b1101001010000000));
}

TEST_F(AddTest, ItReturnsTheCorrectCycles_AddLongs_DataRegisterResult_AddressRegisterSource) {
    ASSERT_EQ(8, subject->execute(0b1101001010001000));
}

TEST_F(AddTest, ItReturnsTheCorrectCycles_AddLongs_DataRegisterResult_ImmediateSource) {
    ASSERT_EQ(16, subject->execute(0b1101001010111100));
}

TEST_F(AddTest, ItReturnsTheCorrectCycles_AddLongs_DataRegisterResult_OtherSource) {
    ASSERT_EQ(14, subject->execute(0b1101001010010000));
}