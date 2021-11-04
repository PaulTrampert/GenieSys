//
// Created by pault on 11/3/2021.
//

#include <GenieSys/CpuOperations/ILLEGAL.h>
#include <gtest/gtest.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>

using namespace GenieSys;

class ILLEGALTest : public ::testing::Test {
public:
    ILLEGAL* subject;
    Bus bus;
    M68kCpu* cpu;

    uint16_t opWord = 0b0100101011111100;

    ILLEGALTest() : ::testing::Test() {
        cpu = bus.getCpu();
        subject = new ILLEGAL(cpu, &bus);
    }

    ~ILLEGALTest() override {
        delete subject;
    }
};

TEST_F(ILLEGALTest, Disassemble) {
    ASSERT_EQ("ILLEGAL", subject->disassemble(opWord));
}

TEST_F(ILLEGALTest, Execute) {
    ASSERT_EQ(34, subject->execute(opWord));
}
