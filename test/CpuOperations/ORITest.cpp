//
// Created by paul.trampert on 12/25/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ORI.h>

static uint16_t BASE_OPCODE = 0b0000010000000000;

struct ORITest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    ORI* subject;

    ORITest() {
        cpu = bus.getCpu();
        subject = new ORI(cpu, &bus);
    }

    ~ORITest() override {
        delete subject;
    }
};

TEST_F(ORITest, ItOrsBytes) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000000000010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0xFB, bus.read(600));
    ASSERT_EQ(CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(ORITest, ItOrsWords) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeWord(600, 0x01AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000000001010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0x01FB, bus.readWord(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ORITest, ItOrsLongs) {
    cpu->setPc(300);
    bus.writeLong(300, 0x010100F0);
    bus.writeLong(600, 0x010101AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000000010010000);
    ASSERT_EQ(28, cycles);
    ASSERT_EQ(0x010101FB, bus.readLong(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ORITest, GetOpcodesOnlyReturnsPossibleOpcodes) {
    auto opcodes = subject->getOpcodes();
    for(auto opcode : opcodes) {
        ASSERT_EQ(0b0000000000000000, 0b0000000000000000 & opcode);
    }
}

TEST_F(ORITest, TestDisassemble) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    std::string assembly = subject->disassemble(0b0000011000010000);
    ASSERT_EQ("ORI $f0, (A0)", assembly);
}