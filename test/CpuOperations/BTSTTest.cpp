//
// Created by paul.trampert on 4/4/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/BTST.h>

struct BTSTTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    BTST* subject;

    BTSTTest() {
        cpu = bus.getCpu();
        subject = new BTST(cpu, &bus);
    }

    ~BTSTTest() override {
        delete subject;
    }
};


TEST_F(BTSTTest, SpecificityIsTen) {
    ASSERT_EQ(10, subject->getSpecificity());
}

TEST_F(BTSTTest, DisassembleImmediate) {
    cpu->setPc(20);
    bus.writeWord(20, 0x0003);

    ASSERT_EQ("BTST $03,D7", subject->disassemble(0b0000100000000111));
}

TEST_F(BTSTTest, DisassembleDataRegister) {
    ASSERT_EQ("BTST D2,D7", subject->disassemble(0b0000010100000111));
}