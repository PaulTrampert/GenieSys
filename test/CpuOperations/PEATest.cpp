//
// Created by pault on 11/2/2021.
//
#include <GenieSys/CpuOperations/PEA.h>
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>

using namespace GenieSys;

class PEATest : public ::testing::Test {
public:
    uint16_t opWord = 0b0100100001010011;

    PEA* subject;
    Bus bus;
    M68kCpu* cpu;

    PEATest() : Test() {
        cpu = bus.getCpu();
        subject = new PEA(cpu, &bus);
    }

    ~PEATest() override {
        delete subject;
    }

    void SetUp() override {
        cpu->setAddressRegister(7, 500);
        cpu->setAddressRegister(3, 0xAABBCCDD);
    }
};


TEST_F(PEATest, Disassemble) {
    ASSERT_EQ("PEA (A3)", subject->disassemble(opWord));
}

TEST_F(PEATest, Execute) {
    ASSERT_EQ(12, subject->execute(opWord));
    ASSERT_EQ(496, cpu->getAddressRegister(7));
    ASSERT_EQ(0xAABBCCDD, bus.readLong(496));
}

TEST_F(PEATest, ExecuteWithDisplacementModeTakes16Cycles) {
    // PEA d(A3) -> 0100 1000 01 101 011
    cpu->setAddressRegister(3, 0x1000);
    bus.writeWord(cpu->getPc(), 0x0010);

    ASSERT_EQ(16, subject->execute(0b0100100001101011));
    ASSERT_EQ(0x1010, bus.readLong(496));
}

TEST_F(PEATest, ExecuteWithIndexModeTakes20Cycles) {
    // PEA d(A3,D0) -> 0100 1000 01 110 011
    cpu->setAddressRegister(3, 0x1000);
    cpu->setDataRegister((uint8_t)0, (uint32_t)0);
    bus.writeWord(cpu->getPc(), 0x0005);

    ASSERT_EQ(20, subject->execute(0b0100100001110011));
}

TEST_F(PEATest, ExecuteWithAbsoluteShortTakes16Cycles) {
    // PEA (xxx).W -> 0100 1000 01 111 000
    bus.writeWord(cpu->getPc(), 0x5000);

    ASSERT_EQ(16, subject->execute(0b0100100001111000));
    ASSERT_EQ(0x5000, bus.readLong(496));
}

TEST_F(PEATest, ExecuteWithAbsoluteLongTakes20Cycles) {
    // PEA (xxx).L -> 0100 1000 01 111 001
    bus.writeLong(cpu->getPc(), 0x00345678);

    ASSERT_EQ(20, subject->execute(0b0100100001111001));
    ASSERT_EQ(0x00345678, bus.readLong(496));
}

TEST_F(PEATest, ExecuteWithProgramCounterDisplacementTakes16Cycles) {
    // PEA d(PC) -> 0100 1000 01 111 010
    bus.writeWord(cpu->getPc(), 0x0020);

    ASSERT_EQ(16, subject->execute(0b0100100001111010));
}

TEST_F(PEATest, ExecuteWithProgramCounterIndexTakes20Cycles) {
    // PEA d(PC,D0) -> 0100 1000 01 111 011
    cpu->setDataRegister((uint8_t)0, (uint32_t)0);
    bus.writeWord(cpu->getPc(), 0x0005);

    ASSERT_EQ(20, subject->execute(0b0100100001111011));
}

TEST_F(PEATest, ExecuteWithDataRegisterDirectTriggersIllegalInstructionTrap) {
    // PEA D3 -> 0100 1000 01 000 011
    bus.writeLong(TV_ILLEGAL_INSTR * 4, 0x1000);

    ASSERT_EQ(34, subject->execute(0b0100100001000011));
    ASSERT_EQ(0x1000, cpu->getPc());
}