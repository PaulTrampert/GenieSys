//
// Created by paul.trampert on 12/25/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/CMPI.h>



static uint16_t BASE_OPCODE = 0b0000010000000000;

struct CMPITest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    CMPI* subject;

    CMPITest() {
        cpu = bus.getCpu();
        subject = new CMPI(cpu, &bus);
    }

    ~CMPITest() override {
        delete subject;
    }
};

TEST_F(CMPITest, ItComparesBytes) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000110000010000);
    ASSERT_EQ(12, cycles);
    ASSERT_EQ(0xAB, bus.read(600));
    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_CARRY | GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(CMPITest, ItComparesWords) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeWord(600, 0x01AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000110001010000);
    ASSERT_EQ(12, cycles);
    ASSERT_EQ(0x01AB, bus.readWord(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(CMPITest, ItComparesLongs) {
    cpu->setPc(300);
    bus.writeLong(300, 0x010100F0);
    bus.writeLong(600, 0x010101AB);
    cpu->setAddressRegister(0, 600);
    uint8_t cycles = subject->execute(0b0000110010010000);
    ASSERT_EQ(20, cycles);
    ASSERT_EQ(0x010101AB, bus.readLong(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(CMPITest, GetOpcodesOnlyReturnsPossibleOpcodes) {
    auto opcodes = subject->getOpcodes();
    for(auto opcode : opcodes) {
        ASSERT_EQ(0b0000110000000000, 0b0000110000000000 & opcode);
    }
}

TEST_F(CMPITest, TestDisassemble) {
    cpu->setPc(300);
    bus.writeWord(300, 0x00F0);
    bus.writeByte(600, 0xAB);
    cpu->setAddressRegister(0, 600);
    std::string assembly = subject->disassemble(0b0000110000010000);
    ASSERT_EQ("CMPI $f0,(A0)", assembly);
}