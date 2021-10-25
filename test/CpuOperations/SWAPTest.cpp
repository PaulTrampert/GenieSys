//
// Created by pault on 10/24/2021.
//
#include <GenieSys/CpuOperations/SWAP.h>
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>

class SWAPTest : public ::testing::Test {
public:
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    GenieSys::SWAP* subject;
    uint16_t opWord = 0b0100100001000010;

    SWAPTest() : Test() {
        cpu = bus.getCpu();
        subject = new GenieSys::SWAP(cpu, &bus);
    }

    ~SWAPTest() override {
        delete subject;
    }
};

TEST_F(SWAPTest, Disassemble) {
    ASSERT_EQ("SWAP D2", subject->disassemble(opWord));
}

TEST_F(SWAPTest, Execute) {
    cpu->setDataRegister(2, 0xBBBBAAAA);
    ASSERT_EQ(4, subject->execute(opWord));
    ASSERT_EQ(0xAAAABBBB, cpu->getDataRegister(2));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}
