//
// Created by paul on 6/5/24.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/BSR.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>

struct BSRTestParams {
    std::string name;
    uint8_t shortDisplacement;
    uint16_t wordDisplacement;
    uint32_t startPc;
    uint32_t expectedPc;
    uint32_t expectedTopStackValue;
    std::string expectedDisassembly;
};

struct BSRTest : public testing::TestWithParam<BSRTestParams> {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    GenieSys::BSR* subject;

    BSRTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::BSR(cpu, &bus);
    }

    ~BSRTest() override {
        delete subject;
    }
};

TEST_P(BSRTest, Execute) {
    BSRTestParams params = GetParam();
    cpu->setStackPointer(1000);
    cpu->setPc(params.startPc);
    uint16_t opWord = 0b0110000100000000 | params.shortDisplacement;
    bus.writeWord(params.startPc, params.wordDisplacement);
    uint8_t cycles = subject->execute(opWord);
    ASSERT_EQ(18, cycles);
    ASSERT_EQ(params.expectedPc, cpu->getPc());
    ASSERT_EQ(params.expectedTopStackValue, bus.readLong(cpu->getAddressRegister(cpu->getUspRegister())));
}

TEST_P(BSRTest, Disassemble) {
    BSRTestParams params = GetParam();
    cpu->setPc(params.startPc);
    uint16_t opWord = 0b0110000100000000 | params.shortDisplacement;
    bus.writeWord(params.startPc, params.wordDisplacement);
    ASSERT_EQ(params.expectedDisassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_SUITE_P(
        BSRTests,
        BSRTest,
        testing::Values(
            BSRTestParams {
                .name = "BSR_ShortDisplacement_PositiveValue",
                .shortDisplacement = 10,
                .wordDisplacement = 0,
                .startPc = 20,
                .expectedPc = 30,
                .expectedTopStackValue = 20,
                .expectedDisassembly = "BSR $000a"
            },
            BSRTestParams {
                .name = "BSR_ShortDisplacement_NegativeValue",
                .shortDisplacement = 0xF6,
                .wordDisplacement = 0,
                .startPc = 20,
                .expectedPc = 10,
                .expectedTopStackValue = 20,
                .expectedDisassembly = "BSR $fff6"
            },
            BSRTestParams {
                .name = "BSR_WordDisplacement_PositiveValue",
                .shortDisplacement = 0,
                .wordDisplacement = 10,
                .startPc = 20,
                .expectedPc = 32,
                .expectedTopStackValue = 22,
                .expectedDisassembly = "BSR $000a"
            },
            BSRTestParams {
                .name = "BSR_WordDisplacement_NegativeValue",
                .shortDisplacement = 0,
                .wordDisplacement = 0xFFF6,
                .startPc = 20,
                .expectedPc = 12,
                .expectedTopStackValue = 22,
                .expectedDisassembly = "BSR $fff6"
            }
        ),
        [](const testing::TestParamInfo<BSRTestParams>& info) {
            return info.param.name;
        }
);