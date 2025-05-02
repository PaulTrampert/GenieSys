//
// Created by paul on 5/2/25.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/DBcc.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include "GenieSys/ConditionCodes.h"

using namespace GenieSys;

struct DBccTestParams {
    std::string testName;
    uint8_t condition;
    uint8_t reg;
    uint16_t data;
    int16_t wordDisplacement;
    uint32_t pc;
    uint8_t ccr;
    uint32_t expectedPc;
    uint8_t expectedCycles;
    std::string expectedDisassembly;
};

struct DBccTest : testing::TestWithParam<DBccTestParams> {
    M68kCpu* cpu;
    Bus bus;
    DBcc* subject;
    uint16_t baseOpcode = 0b0101000011001000;

    DBccTest() {
        cpu = bus.getCpu();
        subject = new DBcc(cpu, &bus);
    }

    ~DBccTest() override {
        delete subject;
    }
};

TEST_P(DBccTest, Execute) {
    DBccTestParams params = GetParam();
    uint16_t opWord = baseOpcode | ((params.condition << 8) & 0b0000111100000000) | (params.reg);
    bus.writeWord(params.pc, params.wordDisplacement);
    cpu->setDataRegister(params.reg, params.data);
    cpu->setPc(params.pc);
    cpu->setCcrFlags(params.ccr);

    ASSERT_EQ(params.expectedCycles, subject->execute(opWord));
    ASSERT_EQ(params.expectedPc, cpu->getPc());
}

TEST_P(DBccTest, Disassemble) {
    DBccTestParams params = GetParam();
    uint16_t opWord = baseOpcode | ((params.condition << 8) & 0b0000111100000000) | ((params.reg << 4) & 0b0000000011110000);
    bus.writeWord(params.pc, params.wordDisplacement);
    cpu->setPc(params.pc);
    cpu->setCcrFlags(params.ccr);

    ASSERT_EQ(params.expectedDisassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_SUITE_P(DBcc, DBccTest, testing::Values(
        DBccTestParams {
            .testName = "DBcc_WhenConditionTrue",
            .condition = CC_EQ,
            .reg = 0,
            .data = 0,
            .wordDisplacement = 10,
            .pc = 1000,
            .ccr = CCR_ZERO,
            .expectedPc = 1002,
            .expectedCycles = 12,
            .expectedDisassembly = "DBEQ D0, $000a"
        },
        DBccTestParams {
            .testName = "DBcc_WhenConditionFalse_AndDataNotNegativeOne",
            .condition = CC_EQ,
            .reg = 0,
            .data = 1,
            .wordDisplacement = 10,
            .pc = 1000,
            .ccr = 0,
            .expectedPc = 1012,
            .expectedCycles = 14,
            .expectedDisassembly = "DBEQ D0, $000a"
        },
        DBccTestParams {
            .testName = "DBcc_WhenConditionFalse_AndDataNegativeOne",
            .condition = CC_EQ,
            .reg = 0,
            .data = 0,
            .wordDisplacement = 10,
            .pc = 1000,
            .ccr = CCR_NEGATIVE | CCR_OVERFLOW,
            .expectedPc = 1002,
            .expectedCycles = 10,
            .expectedDisassembly = "DBEQ D0, $000a"
        }
    ),
    [](const testing::TestParamInfo<DBccTestParams>& info) {
        return info.param.testName;
    }
);
