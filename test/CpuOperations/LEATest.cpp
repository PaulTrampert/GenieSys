//
// Created by copilot on 11/24/2024.
//
#include <GenieSys/CpuOperations/LEA.h>
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>

using namespace GenieSys;

class LEATest : public ::testing::Test {
public:
    // LEA (A3),A5 -> 0100 101 111 010 011 = 0x4BD3
    uint16_t opWord = 0b0100101111010011;

    LEA* subject;
    Bus bus;
    M68kCpu* cpu;

    LEATest() : Test() {
        cpu = bus.getCpu();
        subject = new LEA(cpu, &bus);
    }

    ~LEATest() override {
        delete subject;
    }

    void SetUp() override {
        cpu->setAddressRegister(3, 0xAABBCCDD);
        cpu->setAddressRegister(5, 0);
    }
};


TEST_F(LEATest, Disassemble) {
    ASSERT_EQ("LEA (A3),A5", subject->disassemble(opWord));
}

TEST_F(LEATest, Execute) {
    ASSERT_EQ(4, subject->execute(opWord));
    ASSERT_EQ(0xAABBCCDD, cpu->getAddressRegister(5));
}

TEST_F(LEATest, ExecuteDoesNotAffectSourceRegister) {
    subject->execute(opWord);
    ASSERT_EQ(0xAABBCCDD, cpu->getAddressRegister(3));
}

TEST_F(LEATest, ExecuteWithDataRegisterDirectModeTriggersIllegalInstructionTrap) {
    // LEA D3,A5 -> 0100 101 111 000 011 = EA mode 0b000 (Data register direct)
    uint16_t illegalOpWord = 0b0100101111000011;
    uint32_t trapHandlerAddress = 0x1000;
    bus.writeLong(TV_ILLEGAL_INSTR * 4, trapHandlerAddress);

    ASSERT_EQ(34, subject->execute(illegalOpWord));
    ASSERT_EQ(trapHandlerAddress, cpu->getPc());
}

TEST_F(LEATest, ExecuteWithAddressRegisterDirectModeTriggersIllegalInstructionTrap) {
    // LEA A3,A5 -> 0100 101 111 001 011 = EA mode 0b001 (Address register direct)
    uint16_t illegalOpWord = 0b0100101111001011;
    uint32_t trapHandlerAddress = 0x2000;
    bus.writeLong(TV_ILLEGAL_INSTR * 4, trapHandlerAddress);

    ASSERT_EQ(34, subject->execute(illegalOpWord));
    ASSERT_EQ(trapHandlerAddress, cpu->getPc());
}

TEST_F(LEATest, ExecuteWithPostIncrementModeTriggersIllegalInstructionTrap) {
    // LEA (A3)+,A5 -> 0100 101 111 011 011 = EA mode 0b011 (Address register indirect with post-increment)
    uint16_t illegalOpWord = 0b0100101111011011;
    uint32_t trapHandlerAddress = 0x3000;
    bus.writeLong(TV_ILLEGAL_INSTR * 4, trapHandlerAddress);

    ASSERT_EQ(34, subject->execute(illegalOpWord));
    ASSERT_EQ(trapHandlerAddress, cpu->getPc());
}

TEST_F(LEATest, ExecuteWithPreDecrementModeTriggersIllegalInstructionTrap) {
    // LEA -(A3),A5 -> 0100 101 111 100 011 = EA mode 0b100 (Address register indirect with pre-decrement)
    uint16_t illegalOpWord = 0b0100101111100011;
    uint32_t trapHandlerAddress = 0x4000;
    bus.writeLong(TV_ILLEGAL_INSTR * 4, trapHandlerAddress);

    ASSERT_EQ(34, subject->execute(illegalOpWord));
    ASSERT_EQ(trapHandlerAddress, cpu->getPc());
}

TEST_F(LEATest, ExecuteWithDisplacementModeTakes8Cycles) {
    // LEA d(A3),A5 -> 0100 101 111 101 011 = EA mode 0b101 (Address register indirect with displacement)
    uint16_t opWord = 0b0100101111101011;
    cpu->setAddressRegister(3, 0x1000);
    bus.writeWord(cpu->getPc(), 0x0010); // displacement = 16

    ASSERT_EQ(8, subject->execute(opWord));
    ASSERT_EQ(0x1010, cpu->getAddressRegister(5));
}

TEST_F(LEATest, ExecuteWithIndexModeTakes12Cycles) {
    // LEA d(A3,D0),A5 -> 0100 101 111 110 011 = EA mode 0b110 (Address register indirect with index)
    uint16_t opWord = 0b0100101111110011;
    cpu->setAddressRegister(3, 0x1000);
    cpu->setDataRegister(0, (uint16_t)0x0100);
    bus.writeWord(cpu->getPc(), 0x0005); // extension word: D0.L, no scale, 5 displacement

    ASSERT_EQ(12, subject->execute(opWord));
}

TEST_F(LEATest, ExecuteWithAbsoluteShortTakes8Cycles) {
    // LEA (xxx).W,A5 -> 0100 101 111 111 000 = EA mode 0b111, reg 0b000
    uint16_t opWord = 0b0100101111111000;
    bus.writeWord(cpu->getPc(), 0x5000); // absolute short address

    ASSERT_EQ(8, subject->execute(opWord));
    ASSERT_EQ(0x5000, cpu->getAddressRegister(5));
}

TEST_F(LEATest, ExecuteWithAbsoluteLongTakes12Cycles) {
    // LEA (xxx).L,A5 -> 0100 101 111 111 001 = EA mode 0b111, reg 0b001
    uint16_t opWord = 0b0100101111111001;
    bus.writeLong(cpu->getPc(), 0x12345678); // absolute long address

    ASSERT_EQ(12, subject->execute(opWord));
    ASSERT_EQ(0x12345678, cpu->getAddressRegister(5));
}

TEST_F(LEATest, ExecuteWithPCDisplacementTakes8Cycles) {
    // LEA d(PC),A5 -> 0100 101 111 111 010 = EA mode 0b111, reg 0b010
    uint16_t opWord = 0b0100101111111010;
    uint32_t pcValue = cpu->getPc();
    bus.writeWord(cpu->getPc(), 0x0020); // displacement = 32

    ASSERT_EQ(8, subject->execute(opWord));
    ASSERT_EQ(pcValue + 0x0020, cpu->getAddressRegister(5));
}

TEST_F(LEATest, ExecuteWithPCIndexTakes12Cycles) {
    // LEA d(PC,D0),A5 -> 0100 101 111 111 011 = EA mode 0b111, reg 0b011
    uint16_t opWord = 0b0100101111111011;
    cpu->setDataRegister(0, (uint16_t)0x0100);
    bus.writeWord(cpu->getPc(), 0x0005); // extension word: D0.L, no scale, 5 displacement

    ASSERT_EQ(12, subject->execute(opWord));
}

