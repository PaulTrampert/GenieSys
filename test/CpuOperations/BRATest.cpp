//
// Created by paul on 6/20/24.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/BRA.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <string>

using namespace GenieSys;

const uint16_t BRA_OPCODE = 0b0110000000000000;

struct BRATestParams {
    std::string name;
    uint16_t opWord;
    uint16_t wordDisplacement;
    uint32_t initPc;
    uint32_t expectedPc;
    std::string expectedDisassembly;
};

struct BRATest : testing::TestWithParam<BRATestParams> {
    Bus bus;
    M68kCpu *cpu;
    BRA *subject;

    BRATest() {
        cpu = bus.getCpu();
        subject = new BRA(cpu, &bus);
    }

    ~BRATest() override {
        delete subject;
    }
};

TEST_P(BRATest, Execute) {
    BRATestParams params = GetParam();
    cpu->setPc(params.initPc);
    bus.writeWord(params.initPc, params.wordDisplacement);
    uint8_t cycles = subject->execute(params.opWord);
    ASSERT_EQ(10, cycles);
    ASSERT_EQ(params.expectedPc, cpu->getPc());
}

TEST_P(BRATest, Disassemble) {
    BRATestParams params = GetParam();
    cpu->setPc(params.initPc);
    bus.writeWord(params.initPc, params.wordDisplacement);
    ASSERT_EQ(params.expectedDisassembly, subject->disassemble(params.opWord));
}

INSTANTIATE_TEST_SUITE_P(
        BRATests,
        BRATest,
        testing::Values(
                BRATestParams{
                    .name = "BRA_WhenShortDisplacementIs5",
                    .opWord = BRA_OPCODE | (uint8_t) 5,
                    .wordDisplacement = 0,
                    .initPc = 100,
                    .expectedPc = 105,
                    .expectedDisassembly = "BRA.s #5"
                },
                BRATestParams{
                    .name = "BRA_WhenShortDisplacementIsNegative5",
                    .opWord = BRA_OPCODE | (uint8_t) -5,
                    .wordDisplacement = 0,
                    .initPc = 100,
                    .expectedPc = 95,
                    .expectedDisassembly = "BRA.s #-5"
                },
                BRATestParams{
                    .name = "BRA_WhenWordDisplacementIs5",
                    .opWord = BRA_OPCODE,
                    .wordDisplacement = 5,
                    .initPc = 100,
                    .expectedPc = 107,
                    .expectedDisassembly = "BRA.w #5"
                },
                BRATestParams{
                    .name = "BRA_WhenWordDisplacementIsNegative5",
                    .opWord = BRA_OPCODE,
                    .wordDisplacement = (uint16_t)-5,
                    .initPc = 100,
                    .expectedPc = 97,
                    .expectedDisassembly = "BRA.w #-5"
                }
                ),
        [](const testing::TestParamInfo<BRATestParams>& info) {
            return info.param.name;
        }
);