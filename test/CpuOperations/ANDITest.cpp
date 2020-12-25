//
// Created by paul.trampert on 12/25/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ANDI.h>

uint16_t BASE_OPCODE = 0b0000001000000000;

struct ANDITest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    ANDI* subject;

    ANDITest() {
        cpu = bus.getCpu();
        subject = new ANDI(cpu, &bus);
    }

    ~ANDITest() override {
        delete subject;
    }
};

TEST_F(ANDITest, ItAndsBytes) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000001000010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0xA0, bus.read(600));
    ASSERT_EQ(CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(ANDITest, ItAndsWords) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeWord(600, 0x01AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000001001010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0x00A0, bus.readWord(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ANDITest, ItAndsLongs) {
    cpu->setPc(300);
    bus.writeLong(300, 0x010100F0);
    bus.writeLong(600, 0x010101AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000001010010000);
    ASSERT_EQ(28, cycles);
    ASSERT_EQ(0x010100A0, bus.readLong(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}