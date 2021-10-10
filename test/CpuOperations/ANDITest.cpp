//
// Created by paul.trampert on 12/25/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ANDI.h>
#include <GenieSys/Bus.h>


static uint16_t BASE_OPCODE = 0b0000001000000000;

struct ANDITest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
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
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
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

TEST_F(ANDITest, GetOpcodesOnlyReturnsPossibleOpcodes) {
    auto opcodes = subject->getOpcodes();
    for(auto opcode : opcodes) {
        ASSERT_EQ(0b0000001000000000, 0b0000001000000000 & opcode);
    }
}

TEST_F(ANDITest, TestDisassemble) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    std::string assembly = subject->disassemble(0b0000001000010000);
    ASSERT_EQ("ANDI $f0, (A0)", assembly);
}