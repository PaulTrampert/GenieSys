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
#include <GenieSys/ConditionCodes.h>

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
    std::string expectedDisassembly;
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
    bus.writeWord(params.pc, params.wordDisplacement);
    cpu->setPc(params.pc);
    cpu->setCcrFlags(params.ccr);

    ASSERT_EQ(params.expectedCycles, subject->execute(opWord));
    ASSERT_EQ(params.expectedPc, cpu->getPc());
}

TEST_P(BccTest, Disassemble) {
    BccParams params = GetParam();
    uint16_t opWord = baseOpcode | ((params.condition << 8) & 0b0000111100000000) | (params.byteDisplacement & 0xFF);
    bus.writeWord(params.pc, params.wordDisplacement);
    cpu->setPc(params.pc);
    cpu->setCcrFlags(params.ccr);

    ASSERT_EQ(params.expectedDisassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_SUITE_P(Bcc, BccTest, testing::Values(
        BccParams {
                .testName = "BCC_Byte_WhenCarryClear",
                .condition = CC_CC,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BCC -5"
        },
        BccParams {
                .testName = "BCC_Byte_WhenCarrySet",
                .condition = CC_CC,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_CARRY,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BCC -5"
        },
        BccParams {
                .testName = "BCS_Byte_WhenCarryClear",
                .condition = CC_CS,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BCS -5"
        },
        BccParams {
                .testName = "BCS_Byte_WhenCarrySet",
                .condition = CC_CS,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_CARRY,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BCS -5"
        },
        BccParams {
                .testName = "BEQ_Byte_WhenZeroClear",
                .condition = CC_EQ,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BEQ -5"
        },
        BccParams {
                .testName = "BEQ_Byte_WhenZeroSet",
                .condition = CC_EQ,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_ZERO,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BEQ -5"
        },
        BccParams {
                .testName = "BGE_Byte_WhenPositiveAndOverflow",
                .condition = CC_GE,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_OVERFLOW,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BGE -5"
        },
        BccParams {
                .testName = "BGE_Byte_WhenPositiveAndNoOverflow",
                .condition = CC_GE,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BGE -5"
        },
        BccParams {
            .testName = "BGE_Byte_WhenNegativeAndOverflow",
            .condition = CC_GE,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = CCR_NEGATIVE | CCR_OVERFLOW,
            .expectedPc = 95,
            .expectedCycles = 10,
            .expectedDisassembly = "BGE -5"
        },
        BccParams {
            .testName = "BGE_Byte_WhenNegativeAndNoOverflow",
            .condition = CC_GE,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = CCR_NEGATIVE,
            .expectedPc = 100,
            .expectedCycles = 8,
            .expectedDisassembly = "BGE -5"
        },
        BccParams {
            .testName = "BGT_Byte_WhenZero",
            .condition = CC_GT,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = CCR_ZERO,
            .expectedPc = 100,
            .expectedCycles = 8,
            .expectedDisassembly = "BGT -5"
        },
        BccParams {
            .testName = "BGT_Byte_WhenPositiveAndOverflow",
            .condition = CC_GT,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = CCR_OVERFLOW,
            .expectedPc = 100,
            .expectedCycles = 8,
            .expectedDisassembly = "BGT -5"
        },
        BccParams {
            .testName = "BGT_Byte_WhenPositiveAndNoOverflow",
            .condition = CC_GT,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = 0,
            .expectedPc = 95,
            .expectedCycles = 10,
            .expectedDisassembly = "BGT -5"
        },
        BccParams {
            .testName = "BGT_Byte_WhenNegativeAndOverflow",
            .condition = CC_GT,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = CCR_NEGATIVE | CCR_OVERFLOW,
            .expectedPc = 95,
            .expectedCycles = 10,
            .expectedDisassembly = "BGT -5"
        },
        BccParams {
            .testName = "BGT_Byte_WhenNegativeAndNoOverflow",
            .condition = CC_GT,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = CCR_NEGATIVE,
            .expectedPc = 100,
            .expectedCycles = 8,
            .expectedDisassembly = "BGT -5"
        },
        BccParams {
            .testName = "BHI_Byte_WhenCarryAndZeroClear",
            .condition = CC_HI,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = 0,
            .expectedPc = 95,
            .expectedCycles = 10,
            .expectedDisassembly = "BHI -5"
        },
        BccParams {
            .testName = "BHI_Byte_WhenCarryClearAndZeroSet",
            .condition = CC_HI,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = CCR_ZERO,
            .expectedPc = 100,
            .expectedCycles = 8,
            .expectedDisassembly = "BHI -5"
        },
        BccParams {
                .testName = "BHI_Byte_WhenCarrySetAndZeroClear",
                .condition = CC_HI,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_CARRY,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BHI -5"
        },
        BccParams{
                .testName = "BHI_Byte_WhenCarryAndZeroSet",
                .condition = CC_HI,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_CARRY | CCR_ZERO,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BHI -5"
        },
        BccParams {
                .testName = "BLE_Byte_WhenZero",
                .condition = CC_LE,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_ZERO,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BLE -5"
        },
        BccParams {
                .testName = "BLE_Byte_WhenPositiveAndOverflow",
                .condition = CC_LE,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_OVERFLOW,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BLE -5"
        },
        BccParams {
                .testName = "BLE_Byte_WhenPositiveAndNoOverflow",
                .condition = CC_LE,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BLE -5"
        },
        BccParams {
                .testName = "BLE_Byte_WhenNegativeAndOverflow",
                .condition = CC_LE,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_NEGATIVE | CCR_OVERFLOW,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BLE -5"
        },
        BccParams {
                .testName = "BLE_Byte_WhenNegativeAndNoOverflow",
                .condition = CC_LE,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_NEGATIVE,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BLE -5"
        },
        BccParams {
                .testName = "BLS_Byte_WhenCarryAndZeroClear",
                .condition = CC_LS,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BLS -5"
        },
        BccParams {
                .testName = "BLS_Byte_WhenCarryClearAndZeroSet",
                .condition = CC_LS,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_ZERO,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BLS -5"
        },
        BccParams {
                .testName = "BLS_Byte_WhenCarrySetAndZeroClear",
                .condition = CC_LS,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_CARRY,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BLS -5"
        },
        BccParams {
                .testName = "BLS_Byte_WhenCarryAndZeroSet",
                .condition = CC_LS,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_CARRY | CCR_ZERO,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BLS -5"
        },
        BccParams {
                .testName = "BLT_Byte_WhenNegativeAndNoOverflow",
                .condition = CC_LT,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_NEGATIVE,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BLT -5"
        },
        BccParams {
                .testName = "BLT_Byte_WhenPositiveAndOverflow",
                .condition = CC_LT,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_OVERFLOW,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BLT -5"
        },
        BccParams {
                .testName = "BLT_Byte_WhenPositiveAndNoOverflow",
                .condition = CC_LT,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BLT -5"
        },
        BccParams {
                .testName = "BLT_Byte_WhenNegativeAndOverflow",
                .condition = CC_LT,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_NEGATIVE | CCR_OVERFLOW,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BLT -5"
        },
        BccParams {
                .testName = "BMI_Byte_WhenNegative",
                .condition = CC_MI,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_NEGATIVE,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BMI -5"
        },
        BccParams {
                .testName = "BMI_Byte_WhenPositive",
                .condition = CC_MI,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BMI -5"
        },
        BccParams {
                .testName = "BNE_Byte_WhenZeroClear",
                .condition = CC_NE,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BNE -5"
        },
        BccParams {
                .testName = "BNE_Byte_WhenZeroSet",
                .condition = CC_NE,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_ZERO,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BNE -5"
        },
        BccParams {
                .testName = "BPL_Byte_WhenPositive",
                .condition = CC_PL,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = 0,
                .expectedPc = 95,
                .expectedCycles = 10,
                .expectedDisassembly = "BPL -5"
        },
        BccParams {
                .testName = "BPL_Byte_WhenNegative",
                .condition = CC_PL,
                .byteDisplacement = -5,
                .wordDisplacement = 0,
                .pc = 100,
                .ccr = CCR_NEGATIVE,
                .expectedPc = 100,
                .expectedCycles = 8,
                .expectedDisassembly = "BPL -5"
        },
        BccParams {
            .testName = "BVS_Byte_WhenOverflowSet",
            .condition = CC_VS,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = CCR_OVERFLOW,
            .expectedPc = 95,
            .expectedCycles = 10,
            .expectedDisassembly = "BVS -5"
        },
        BccParams {
            .testName = "BVS_Byte_WhenOverflowClear",
            .condition = CC_VS,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = 0,
            .expectedPc = 100,
            .expectedCycles = 8,
            .expectedDisassembly = "BVS -5"
        },
        BccParams {
            .testName = "BVC_Byte_WhenOverflowClear",
            .condition = CC_VC,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = 0,
            .expectedPc = 95,
            .expectedCycles = 10,
            .expectedDisassembly = "BVC -5"
        },
        BccParams {
            .testName = "BVC_Byte_WhenOverflowSet",
            .condition = CC_VC,
            .byteDisplacement = -5,
            .wordDisplacement = 0,
            .pc = 100,
            .ccr = CCR_OVERFLOW,
            .expectedPc = 100,
            .expectedCycles = 8,
            .expectedDisassembly = "BVC -5"
        },
        BccParams {
            .testName = "BCC_Word_WhenCarryClear",
            .condition = CC_CC,
            .byteDisplacement = 0,
            .wordDisplacement = -5,
            .pc = 100,
            .ccr = 0,
            .expectedPc = 97,
            .expectedCycles = 10,
            .expectedDisassembly = "BCC -5"
        },
        BccParams {
            .testName = "BCC_Word_WhenCarrySet",
            .condition = CC_CC,
            .byteDisplacement = 0,
            .wordDisplacement = -5,
            .pc = 100,
            .ccr = CCR_CARRY,
            .expectedPc = 102,
            .expectedCycles = 12,
            .expectedDisassembly = "BCC -5"
        }
        ),
     [](const ::testing::TestParamInfo<BccParams>& info) {
         return info.param.testName;
     });