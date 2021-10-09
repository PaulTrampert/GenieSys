//
// Created by paul.trampert on 12/25/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ADDI.h>



static uint16_t BASE_OPCODE = 0b0000010000000000;

struct ADDITest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    ADDI* subject;

    ADDITest() {
        cpu = bus.getCpu();
        subject = new ADDI(cpu, &bus);
    }

    ~ADDITest() override {
        delete subject;
    }
};

TEST_F(ADDITest, ItAddsBytes) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000011000010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0x9B, bus.read(600));
    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY | GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(ADDITest, ItAddsWords) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeWord(600, 0x01AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000011001010000);
    ASSERT_EQ(16, cycles);
    ASSERT_EQ(0x029B, bus.readWord(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ADDITest, ItAddsLongs) {
    cpu->setPc(300);
    bus.writeLong(300, 0x010100F0);
    bus.writeLong(600, 0x010101AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000011010010000);
    ASSERT_EQ(28, cycles);
    ASSERT_EQ(0x0202029B, bus.readLong(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ADDITest, GetOpcodesOnlyReturnsPossibleOpcodes) {
    auto opcodes = subject->getOpcodes();
    for(auto opcode : opcodes) {
        ASSERT_EQ(0b0000011000000000, 0b0000011000000000 & opcode);
    }
}

TEST_F(ADDITest, TestDisassemble) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    std::string assembly = subject->disassemble(0b0000011000010000);
    ASSERT_EQ("ADDI $f0,(A0)", assembly);
}