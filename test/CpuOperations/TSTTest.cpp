//
// Created by paul on 11/21/21.
//
#include <GenieSys/CpuOperations/TST.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/BitMask.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <gtest/gtest.h>

using namespace GenieSys;

struct TSTTestParams {
    std::string name;
    std::vector<uint8_t> data;
    uint8_t size;
    uint8_t eaModeId;
    uint8_t eaReg;
    uint8_t ccr;
    uint8_t expectedCcr;
    uint8_t expectedCycles;
    std::string expectedDisassembly;
};

class TSTTest : public ::testing::TestWithParam<TSTTestParams> {
public:
    uint16_t opWordBase = 0b0100101000000000;
    BitMask<uint16_t> sizeMask = BitMask<uint16_t>(7, 2, 0, 2);
    BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
    BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    M68kCpu* cpu;
    Bus bus;
    TST* subject;

    TSTTest() : TestWithParam() {
        cpu = bus.getCpu();
        subject = new TST(cpu, &bus);
    }

    ~TSTTest() override {
        delete subject;
    }
};

TEST_P(TSTTest, Execute) {
    TSTTestParams params = GetParam();
    cpu->setCcrFlags(params.ccr);
    cpu->getAddressingMode(params.eaModeId)->getData(params.eaReg, 1 << params.size)->write(params.data);
    uint16_t opWord = eaRegMask.compose(
            eaModeMask.compose(
                    sizeMask.compose(opWordBase, params.size), params.eaModeId), params.eaReg);
    ASSERT_EQ(params.expectedCycles, subject->execute(opWord));
    ASSERT_EQ(params.expectedCcr, cpu->getCcrFlags());
}

TEST_P(TSTTest, Disassemble) {
    TSTTestParams params = GetParam();
    cpu->setCcrFlags(params.ccr);
    cpu->getAddressingMode(params.eaModeId)->getData(params.eaReg, 1 << params.size)->write(params.data);
    uint16_t opWord = eaRegMask.compose(
            eaModeMask.compose(
                    sizeMask.compose(opWordBase, params.size), params.eaModeId), params.eaReg);
    ASSERT_EQ(params.expectedDisassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_CASE_P(TST, TSTTest,
    testing::Values(
        TSTTestParams {
            .name = "TST_Positive_Byte_With_Empty_Ccr",
            .data = std::vector {(uint8_t)0x01},
            .size = 0,
            .eaModeId = 0,
            .eaReg = 1,
            .ccr = 0,
            .expectedCcr = 0,
            .expectedCycles = 4,
            .expectedDisassembly = "TST.b D1"
        },
        TSTTestParams {
            .name = "TST_Negative_Byte_With_Empty_Ccr",
            .data = std::vector {(uint8_t)0x81},
            .size = 0,
            .eaModeId = 0,
            .eaReg = 1,
            .ccr = 0,
            .expectedCcr = CCR_NEGATIVE,
            .expectedCycles = 4,
            .expectedDisassembly = "TST.b D1"
        },
        TSTTestParams {
            .name = "TST_Zero_Byte_With_Empty_Ccr",
            .data = std::vector {(uint8_t)0},
            .size = 0,
            .eaModeId = 0,
            .eaReg = 1,
            .ccr = 0,
            .expectedCcr = CCR_ZERO,
            .expectedCycles = 4,
            .expectedDisassembly = "TST.b D1"
        },
        TSTTestParams {
                .name = "TST_Positive_Byte_With_Full_Ccr",
                .data = std::vector {(uint8_t)0x01},
                .size = 0,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0xFF,
                .expectedCcr = CCR_EXTEND,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.b D1"
        },
        TSTTestParams {
                .name = "TST_Negative_Byte_With_Full_Ccr",
                .data = std::vector {(uint8_t)0x81},
                .size = 0,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0xFF,
                .expectedCcr = CCR_EXTEND | CCR_NEGATIVE,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.b D1"
        },
        TSTTestParams {
                .name = "TST_Zero_Byte_With_Full_Ccr",
                .data = std::vector {(uint8_t)0},
                .size = 0,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0xFF,
                .expectedCcr = CCR_EXTEND | CCR_ZERO,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.b D1"
        },
        TSTTestParams {
                .name = "TST_Positive_Word_With_Empty_Ccr",
                .data = std::vector {(uint8_t)0x00, (uint8_t) 0x01},
                .size = 1,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0,
                .expectedCcr = 0,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.w D1"
        },
        TSTTestParams {
                .name = "TST_Negative_Word_With_Empty_Ccr",
                .data = std::vector {(uint8_t)0x80, (uint8_t) 0x01},
                .size = 1,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0,
                .expectedCcr = CCR_NEGATIVE,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.w D1"
        },
        TSTTestParams {
                .name = "TST_Zero_Word_With_Empty_Ccr",
                .data = std::vector {(uint8_t)0x00, (uint8_t) 0x00},
                .size = 1,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0,
                .expectedCcr = CCR_ZERO,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.w D1"
        },
        TSTTestParams {
                .name = "TST_Positive_Word_With_Full_Ccr",
                .data = std::vector {(uint8_t)0x00, (uint8_t) 0x01},
                .size = 1,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0xFF,
                .expectedCcr = CCR_EXTEND,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.w D1"
        },
        TSTTestParams {
                .name = "TST_Negative_Word_With_Full_Ccr",
                .data = std::vector {(uint8_t)0x80, (uint8_t) 0x01},
                .size = 1,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0xFF,
                .expectedCcr = CCR_EXTEND | CCR_NEGATIVE,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.w D1"
        },
        TSTTestParams {
                .name = "TST_Zero_Word_With_Full_Ccr",
                .data = std::vector {(uint8_t)0x00, (uint8_t) 0x00},
                .size = 1,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0xFF,
                .expectedCcr = CCR_EXTEND | CCR_ZERO,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.w D1"
        },
        TSTTestParams {
                .name = "TST_Positive_Long_With_Empty_Ccr",
                .data = std::vector {(uint8_t)0x00, (uint8_t)0x00, (uint8_t)0x00, (uint8_t) 0x01},
                .size = 2,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0,
                .expectedCcr = 0,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.l D1"
        },
        TSTTestParams {
                .name = "TST_Negative_Long_With_Empty_Ccr",
                .data = std::vector {(uint8_t)0x80, (uint8_t)0x00, (uint8_t)0x00, (uint8_t) 0x01},
                .size = 2,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0,
                .expectedCcr = CCR_NEGATIVE,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.l D1"
        },
        TSTTestParams {
                .name = "TST_Zero_Long_With_Empty_Ccr",
                .data = std::vector {(uint8_t)0x00, (uint8_t)0x00, (uint8_t)0x00, (uint8_t) 0x00},
                .size = 2,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0,
                .expectedCcr = CCR_ZERO,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.l D1"
        },
        TSTTestParams {
                .name = "TST_Positive_Long_With_Full_Ccr",
                .data = std::vector {(uint8_t)0x00, (uint8_t)0x00, (uint8_t)0x00, (uint8_t) 0x01},
                .size = 2,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0xFF,
                .expectedCcr = CCR_EXTEND,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.l D1"
        },
        TSTTestParams {
                .name = "TST_Negative_Long_With_Full_Ccr",
                .data = std::vector {(uint8_t)0x80, (uint8_t)0x00, (uint8_t)0x00, (uint8_t) 0x01},
                .size = 2,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0xFF,
                .expectedCcr = CCR_EXTEND | CCR_NEGATIVE,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.l D1"
        },
        TSTTestParams {
                .name = "TST_Zero_Long_With_Full_Ccr",
                .data = std::vector {(uint8_t)0x00, (uint8_t)0x00, (uint8_t)0x00, (uint8_t) 0x00},
                .size = 2,
                .eaModeId = 0,
                .eaReg = 1,
                .ccr = 0xFF,
                .expectedCcr = CCR_EXTEND | CCR_ZERO,
                .expectedCycles = 4,
                .expectedDisassembly = "TST.l D1"
        }
    ),
    [](const ::testing::TestParamInfo<TSTTestParams>& info) {
        return info.param.name;
    }
);