//
// Created by paul on 5/28/24.
//
#include <cstdint>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/Bcc.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>

using namespace GenieSys;

struct BccParams {
    std::string testName;
    uint8_t condition;
    int8_t byteDisplacement;
    int16_t wordDisplacement;
    uint32_t pc;
    uint8_t ccr;
    uint32_t expectedPc;
    uint8_t expectedCycles;
    std::string conditionName;
};

struct BccTest : testing::TestWithParam<BccParams> {
    M68kCpu* cpu;
    Bus bus;
    Bcc* subject;
    uint16_t baseOpcode = 0b0110000000000000;

    BccTest() {
        cpu = bus.getCpu();
        subject = new Bcc(cpu, &bus);
    }

    ~BccTest() override {
        delete subject;
    }
};

TEST_P(BccTest, Execute) {
    BccParams params = GetParam();
    uint16_t opWord = baseOpcode | ((params.condition << 8) & 0b0000111100000000) | (params.byteDisplacement & 0xFF);
    bus.writeWord(params.pc, opWord);
    bus.writeWord(params.pc + 2, params.wordDisplacement);
    cpu->setPc(params.pc);
    cpu->setCcrFlags(params.ccr);

    ASSERT_EQ(params.expectedCycles, subject->execute(opWord));
    ASSERT_EQ(params.expectedPc, cpu->getPc());
}

INSTANTIATE_TEST_SUITE_P(Bcc, BccTest, testing::Values(
        BccParams {
                .testName = "BCC -5 when Carry is clear",
                .condition = 0b0100,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0b00000000,
                .expectedPc = 95,
                .expectedCycles = 10,
                .conditionName = "CC"
        },
        BccParams {
                .testName = "BCC -5",
                .condition = 0b0100,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0b00000001,
                .expectedPc = 100,
                .expectedCycles = 8,
                .conditionName = "CC"
        }
        ));