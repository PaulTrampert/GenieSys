//
// Created by paul on 5/18/24.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/MOVEM.h>
#include "GenieSys/Bus.h"
#include "GenieSys/M68kCpu.h"

using namespace GenieSys;

struct MOVEMTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    MOVEM* subject;
    uint16_t baseOpWord = 0b0100100010000000;
    uint16_t regListWord = 0b1111111111111111;

    MOVEMTest() {
        cpu = bus.getCpu();
        subject = new MOVEM(cpu, &bus);
    }

    ~MOVEMTest() override {
        delete subject;
    }

    void SetUp() override {
        bus.writeWord(0, regListWord);
    }
};

TEST_F(MOVEMTest, DisassembleMemoryToRegisterWord) {
    auto word = baseOpWord | 0b0000000000010000;
    ASSERT_EQ("MOVEM.W D0-D7/A0-A7,(A0)", subject->disassemble(word));
}

TEST_F(MOVEMTest, DisassembleMemoryToRegisterLong) {
    auto word = baseOpWord | 0b0000000001010000;
    ASSERT_EQ("MOVEM.L D0-D7/A0-A7,(A0)", subject->disassemble(word));
}

TEST_F(MOVEMTest, DisassembleRegisterToMemoryWord) {
    auto word = baseOpWord | 0b0000010000010000;
    ASSERT_EQ("MOVEM.W (A0),D0-D7/A0-A7", subject->disassemble(word));
}

TEST_F(MOVEMTest, DisassembleRegisterToMemoryLong) {
    auto word = baseOpWord | 0b0000010001010000;
    ASSERT_EQ("MOVEM.L (A0),D0-D7/A0-A7", subject->disassemble(word));
}

TEST_F(MOVEMTest, DisassembleMemoryToRegisterWordReversed) {
    auto word = baseOpWord | 0b0000000000100000;
    bus.writeWord(0, 0xFF00);
    ASSERT_EQ("MOVEM.W D0-D7,-(A0)", subject->disassemble(word));
}