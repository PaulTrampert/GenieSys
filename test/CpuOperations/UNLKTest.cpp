//
// Created by paul on 12/11/21.
//
#include <GenieSys/CpuOperations/UNLK.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <gtest/gtest.h>

using namespace GenieSys;

class UNLKTest : public testing::Test {
public:
    uint16_t baseOpWord = 0b0100111001011000;
    BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);

    Bus bus;
    M68kCpu * cpu;
    UNLK * subject;

    UNLKTest() : Test() {
        cpu = bus.getCpu();
        subject = new UNLK(cpu, &bus);
    }

    ~UNLKTest() override {
        delete subject;
    }
};

TEST_F(UNLKTest, Execute) {
    bus.writeLong(700, 0xAABBCCDD);
    cpu->setAddressRegister(3, 700);
    uint16_t opWord = regMask.compose(baseOpWord, 3);

    ASSERT_EQ(12, subject->execute(opWord));
    ASSERT_EQ(0xAABBCCDD, cpu->getAddressRegister(3));
    ASSERT_EQ(704, cpu->getStackPointer());
}

TEST_F(UNLKTest, Disassemble) {
    uint16_t opWord = regMask.compose(baseOpWord, 3);

    ASSERT_EQ("UNLK A3", subject->disassemble(opWord));
}