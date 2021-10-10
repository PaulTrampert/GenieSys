//
// Created by paul.trampert on 12/6/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/Abcd.h>
#include <GenieSys/Bus.h>



struct AbcdTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    GenieSys::Abcd* subject;

    AbcdTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::Abcd(cpu, &bus);
    }

    ~AbcdTest() override {
        delete subject;
    }
};

TEST_F(AbcdTest, ItCorrectlyAddsDecimalsFromRegistersWithNoCarry) {
    cpu->setDataRegister(2, (uint8_t)0x11);
    cpu->setDataRegister(3, (uint8_t)0x11);
    subject->execute(0b1100010100000011);

    ASSERT_EQ(0x22, cpu->getDataRegister(2));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(AbcdTest, ItCorrectlyDisassemblesDecimalsFromRegisters) {
    ASSERT_EQ("ABCD D3,D2", subject->disassemble(0b1100010100000011));
}

TEST_F(AbcdTest, ItCorrectlyAddsDecimalsFromMemoryWithNoCarry) {
    cpu->setAddressRegister(2, 0x20);
    cpu->setAddressRegister(3, 0x30);
    bus.writeByte(0x1F, 0x11);
    bus.writeByte(0x2F, 0x11);
    subject->execute(0b1100010100001011);

    ASSERT_EQ(0x22, bus.read(0x1F));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(AbcdTest, ItCorrectlyDisassemblesDecimalsFromMemory) {
    ASSERT_EQ("ABCD -(A3),-(A2)", subject->disassemble(0b1100010100001011));
}

TEST_F(AbcdTest, ItCorrectlyAddsDecimalsFromRegistersWithCarry) {
    cpu->setDataRegister(2, (uint8_t)0x88);
    cpu->setDataRegister(3, (uint8_t)0x11);
    cpu->setCcrFlags(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY);

    subject->execute(0b1100010100000011);

    ASSERT_EQ(0x00, cpu->getDataRegister(2));
    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY | GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(AbcdTest, ItCorrectlyAddsDecimalsFromMemoryWithCarry) {
    cpu->setAddressRegister(2, 0x20);
    cpu->setAddressRegister(3, 0x30);
    cpu->setCcrFlags(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY);
    bus.writeByte(0x1F, 0x88);
    bus.writeByte(0x2F, 0x11);

    subject->execute(0b1100010100001011);

    ASSERT_EQ(0x00, bus.read(0x1F));
    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY | GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(AbcdTest, ItReturnsTheCorrectClockCycles_ForDataRegisters) {
    ASSERT_EQ(6, subject->execute(0b1100010100000011));
}

TEST_F(AbcdTest, ItReturnsTheCorrectClockCycles_ForAddressRegisterPreDecrement) {
    ASSERT_EQ(18, subject->execute(0b1100010100001011));
}