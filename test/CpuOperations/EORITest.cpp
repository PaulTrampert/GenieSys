//
// Created by paul.trampert on 12/25/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/EORI.h>

struct EORITest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    EORI* subject;

    EORITest() {
        cpu = bus.getCpu();
        subject = new EORI(cpu, &bus);
    }

    ~EORITest() override {
        delete subject;
    }
};

TEST_F(EORITest, ItEorsBytes) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000101000010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0x5B, bus.read(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(EORITest, ItEorsWords) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeWord(600, 0x01AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000101001010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0x015B, bus.readWord(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(EORITest, ItEorsLongs) {
    cpu->setPc(300);
    bus.writeLong(300, 0x010100F0);
    bus.writeLong(600, 0x010101AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000101010010000);
    ASSERT_EQ(28, cycles);
    ASSERT_EQ(0x0000015B, bus.readLong(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(EORITest, GetOpcodesOnlyReturnsPossibleOpcodes) {
    auto opcodes = subject->getOpcodes();
    for(auto opcode : opcodes) {
        ASSERT_EQ(0b0000101000000000, 0b0000101000000000 & opcode);
    }
}

TEST_F(EORITest, TestDisassemble) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    std::string assembly = subject->disassemble(0b0000101000010000);
    ASSERT_EQ("EORI $f0, (A0)", assembly);
}