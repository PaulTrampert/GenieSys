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

TEST_F(AddTest, ItDisassembles_DataRegResult) {
    ASSERT_EQ("ADD.b D0,D1", subject->disassemble(0b1101001000000000));
}

TEST_F(AddTest, ItAddsTwoBytes_WhenOneIsNegative) {
    cpu->setDataRegister(0, (uint8_t)55);
    cpu->setDataRegister(1, (uint8_t)-20);

    subject->execute(0b1101001000000000);

    ASSERT_EQ(35, cpu->getDataRegister(1));
    ASSERT_EQ(CCR_OVERFLOW, cpu->getCcrFlags());
}

TEST_F(AddTest, ItCorrectlySetsCarryFlag) {
    cpu->setDataRegister(0, (uint8_t)127);
    cpu->setDataRegister(1, (uint8_t)1);

    subject->execute(0b1101001000000000);

    ASSERT_EQ(128, cpu->getDataRegister(1));
    ASSERT_EQ(CCR_EXTEND | CCR_CARRY | CCR_NEGATIVE, cpu->getCcrFlags());
}