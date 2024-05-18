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

    MOVEMTest() {
        cpu = bus.getCpu();
        subject = new MOVEM(cpu, &bus);
    }

    ~MOVEMTest() override {
        delete subject;
    }
};

TEST_F(MOVEMTest, DisassembleAddressRegisterMode) {
    auto word = baseOpWord | 0b0000000000010000;
    ASSERT_EQ("MOVEM.W $0000,(A0)", subject->disassemble(word));
}