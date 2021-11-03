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