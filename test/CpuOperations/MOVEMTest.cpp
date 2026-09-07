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

// The MOVEM timing table is a per addressing mode base plus 4 cycles per word, or 8 cycles per
// long word, transferred. The effective address calculation is already covered by the base.
struct MOVEMCycleTestParams {
    std::string testName;
    uint16_t opWord;
    uint16_t regListWord;
    uint8_t expectedCycles;
};

struct MOVEMCycleTest : testing::TestWithParam<MOVEMCycleTestParams> {
    M68kCpu* cpu;
    Bus bus;
    MOVEM* subject;

    MOVEMCycleTest() {
        cpu = bus.getCpu();
        subject = new MOVEM(cpu, &bus);
    }

    ~MOVEMCycleTest() override {
        delete subject;
    }

    void SetUp() override {
        cpu->setPc(0x100);
        cpu->setAddressRegister(0, 0x400);
        cpu->setDataRegister((uint8_t)0, (uint32_t)0);
        bus.writeWord(0x100, GetParam().regListWord);
    }
};

TEST_P(MOVEMCycleTest, Execute) {
    ASSERT_EQ(GetParam().expectedCycles, subject->execute(GetParam().opWord));
}

INSTANTIATE_TEST_SUITE_P(MOVEM, MOVEMCycleTest, testing::Values(
        MOVEMCycleTestParams {
            // MOVEM.W (A0),D0/D1 -> 0100 1100 10 010 000
            .testName = "MemoryToRegisterWordIndirect",
            .opWord = 0b0100110010010000,
            .regListWord = 0b0000000000000011,
            .expectedCycles = 12 + 4 * 2
        },
        MOVEMCycleTestParams {
            // MOVEM.L (A0),D0/D1 -> 0100 1100 11 010 000
            .testName = "MemoryToRegisterLongIndirect",
            .opWord = 0b0100110011010000,
            .regListWord = 0b0000000000000011,
            .expectedCycles = 12 + 8 * 2
        },
        MOVEMCycleTestParams {
            // MOVEM.W (A0)+,D0-D3 -> 0100 1100 10 011 000
            .testName = "MemoryToRegisterWordPostIncrement",
            .opWord = 0b0100110010011000,
            .regListWord = 0b0000000000001111,
            .expectedCycles = 12 + 4 * 4
        },
        MOVEMCycleTestParams {
            // MOVEM.W d(A0),D0/D1 -> 0100 1100 10 101 000
            .testName = "MemoryToRegisterWordDisplacement",
            .opWord = 0b0100110010101000,
            .regListWord = 0b0000000000000011,
            .expectedCycles = 16 + 4 * 2
        },
        MOVEMCycleTestParams {
            // MOVEM.W D0/D1,(A0) -> 0100 1000 10 010 000
            .testName = "RegisterToMemoryWordIndirect",
            .opWord = 0b0100100010010000,
            .regListWord = 0b0000000000000011,
            .expectedCycles = 8 + 4 * 2
        },
        MOVEMCycleTestParams {
            // MOVEM.L D0/D1,(A0) -> 0100 1000 11 010 000
            .testName = "RegisterToMemoryLongIndirect",
            .opWord = 0b0100100011010000,
            .regListWord = 0b0000000000000011,
            .expectedCycles = 8 + 8 * 2
        },
        MOVEMCycleTestParams {
            // MOVEM.W D0/D1,-(A0) -> 0100 1000 10 100 000
            .testName = "RegisterToMemoryWordPreDecrement",
            .opWord = 0b0100100010100000,
            .regListWord = 0b0000000000000011,
            .expectedCycles = 8 + 4 * 2
        },
        MOVEMCycleTestParams {
            // MOVEM.W D0/D1,d(A0) -> 0100 1000 10 101 000
            .testName = "RegisterToMemoryWordDisplacement",
            .opWord = 0b0100100010101000,
            .regListWord = 0b0000000000000011,
            .expectedCycles = 12 + 4 * 2
        },
        MOVEMCycleTestParams {
            // An empty register list still costs the base, and must never report zero cycles.
            .testName = "EmptyRegisterListCostsTheBase",
            .opWord = 0b0100110010010000,
            .regListWord = 0,
            .expectedCycles = 12
        }
    ),
    [](const testing::TestParamInfo<MOVEMCycleTestParams>& info) {
        return info.param.testName;
    }
);
