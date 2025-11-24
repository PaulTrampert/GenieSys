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
    ASSERT_EQ(12, subject->execute(opWord));
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
