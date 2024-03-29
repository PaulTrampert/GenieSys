//
// Created by paul.trampert on 12/6/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ABCD.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>


struct ABCDTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    GenieSys::ABCD* subject;

    ABCDTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::ABCD(cpu, &bus);
    }

    ~ABCDTest() override {
        delete subject;
    }
};

TEST_F(ABCDTest, ItCorrectlyAddsDecimalsFromRegistersWithNoCarry) {
    cpu->setDataRegister(2, (uint8_t)0x11);
    cpu->setDataRegister(3, (uint8_t)0x11);
    subject->execute(0b1100010100000011);

    ASSERT_EQ(0x22, cpu->getDataRegister(2));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ABCDTest, ItCorrectlyDisassemblesDecimalsFromRegisters) {
    ASSERT_EQ("ABCD D3,D2", subject->disassemble(0b1100010100000011));
}

TEST_F(ABCDTest, ItCorrectlyAddsDecimalsFromMemoryWithNoCarry) {
    cpu->setAddressRegister(2, 0x20);
    cpu->setAddressRegister(3, 0x30);
    bus.writeByte(0x1F, 0x11);
    bus.writeByte(0x2F, 0x11);
    subject->execute(0b1100010100001011);

    ASSERT_EQ(0x22, bus.read(0x1F));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ABCDTest, ItCorrectlyDisassemblesDecimalsFromMemory) {
    ASSERT_EQ("ABCD -(A3),-(A2)", subject->disassemble(0b1100010100001011));
}

TEST_F(ABCDTest, ItCorrectlyAddsDecimalsFromRegistersWithCarry) {
    cpu->setDataRegister(2, (uint8_t)0x88);
    cpu->setDataRegister(3, (uint8_t)0x11);
    cpu->setCcrFlags(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY);

    subject->execute(0b1100010100000011);

    ASSERT_EQ(0x00, cpu->getDataRegister(2));
    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY | GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(ABCDTest, ItCorrectlyAddsDecimalsFromMemoryWithCarry) {
    cpu->setAddressRegister(2, 0x20);
    cpu->setAddressRegister(3, 0x30);
    cpu->setCcrFlags(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY);
    bus.writeByte(0x1F, 0x88);
    bus.writeByte(0x2F, 0x11);

    subject->execute(0b1100010100001011);

    ASSERT_EQ(0x00, bus.read(0x1F));
    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY | GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(ABCDTest, ItReturnsTheCorrectClockCycles_ForDataRegisters) {
    ASSERT_EQ(6, subject->execute(0b1100010100000011));
}

TEST_F(ABCDTest, ItReturnsTheCorrectClockCycles_ForAddressRegisterPreDecrement) {
    ASSERT_EQ(18, subject->execute(0b1100010100001011));
}