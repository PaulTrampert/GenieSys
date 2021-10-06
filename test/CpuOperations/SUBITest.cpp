//
// Created by paul.trampert on 12/25/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/SUBI.h>

static uint16_t BASE_OPCODE = 0b0000010000000000;

struct SUBITest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    SUBI* subject;

    SUBITest() {
        cpu = bus.getCpu();
        subject = new SUBI(cpu, &bus);
    }

    ~SUBITest() override {
        delete subject;
    }
};

TEST_F(SUBITest, ItSubtractsBytes) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000010000010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0xBB, bus.read(600));
    ASSERT_EQ(CCR_EXTEND | CCR_CARRY | CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(SUBITest, ItSubtractsWords) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeWord(600, 0x01AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000010001010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0x00BB, bus.readWord(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(SUBITest, ItSubtractsLongs) {
    cpu->setPc(300);
    bus.writeLong(300, 0x010100F0);
    bus.writeLong(600, 0x010101AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000010010010000);
    ASSERT_EQ(28, cycles);
    ASSERT_EQ(0x000000BB, bus.readLong(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(SUBITest, GetOpcodesOnlyReturnsPossibleOpcodes) {
    auto opcodes = subject->getOpcodes();
    for(auto opcode : opcodes) {
        ASSERT_EQ(0b0000010000000000, 0b0000010000000000 & opcode);
    }
}

TEST_F(SUBITest, TestDisassemble) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    std::string assembly = subject->disassemble(0b0000011000010000);
    ASSERT_EQ("SUBI $f0,(A0)", assembly);
}