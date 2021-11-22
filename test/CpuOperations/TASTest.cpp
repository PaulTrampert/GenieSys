//
// Created by paul on 11/18/21.
//
#include <GenieSys/CpuOperations/TAS.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <gtest/gtest.h>

using namespace GenieSys;

struct TASTestParams {
    std::string name;
    uint8_t data{};
    uint8_t ccr{};
    uint8_t eaMode{};
    uint8_t eaReg{};
    uint8_t expectedCycles{};
    uint8_t expectedResultData{};
    uint8_t expectedCcr{};
    std::string expectedDisassembly;
};

class TASTest : public ::testing::TestWithParam<TASTestParams> {
public:
    Bus bus;
    M68kCpu* cpu;
    TAS* subject;
    BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
    BitMask<uint16_t> eaRegMask = BitMask<uint16_t>(2, 3);

    uint16_t opWordBase = 0b0100101011000000;

    TASTest() : TestWithParam() {
        cpu = bus.getCpu();
        subject = new TAS(cpu, &bus);
    }

    ~TASTest() override {
        delete subject;
    }
};

TEST_P(TASTest, Execute) {
    auto params = GetParam();
    uint16_t opWord = eaModeMask.compose(eaRegMask.compose(opWordBase, params.eaReg), params.eaMode);
    cpu->setAddressRegister(params.eaReg, 500);
    cpu->setCcrFlags(params.ccr);
    cpu->getAddressingMode(params.eaMode)->getData(params.eaReg, 1)->write(params.data);

    ASSERT_EQ(params.expectedCycles, subject->execute(opWord));
    ASSERT_EQ(params.expectedCcr, cpu->getCcrFlags());
    ASSERT_EQ(params.expectedResultData, cpu->getAddressingMode(params.eaMode)->getData(params.eaReg, 1)->getDataAsByte());
}

TEST_P(TASTest, Disassemble) {
    auto param = GetParam();
    uint16_t opWord = eaModeMask.compose(eaRegMask.compose(opWordBase, param.eaReg), param.eaMode);
    ASSERT_EQ(param.expectedDisassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_SUITE_P(TAS, TASTest,
    testing::Values(
            TASTestParams {
                .name = "Data_register_EA_with_value_0_and_cleared_CCR",
                .data = 0,
                .ccr = 0,
                .eaMode = 0,
                .eaReg = 1,
                .expectedCycles = 4,
                .expectedResultData = 0b10000000,
                .expectedCcr = CCR_ZERO,
                .expectedDisassembly = "TAS D1"
            },
            TASTestParams {
                .name = "Data_register_EA_with_negative_value_and_cleared_CCR",
                .data = 0xF4,
                .ccr = 0,
                .eaMode = 0,
                .eaReg = 1,
                .expectedCycles = 4,
                .expectedResultData = 0xF4,
                .expectedCcr = CCR_NEGATIVE,
                .expectedDisassembly = "TAS D1"
            },
            TASTestParams {
                .name = "Data_register_EA_with_positive_value_and_cleared_CCR",
                .data = 0x04,
                .ccr = 0,
                .eaMode = 0,
                .eaReg = 1,
                .expectedCycles = 4,
                .expectedResultData = 0x84,
                .expectedCcr = 0,
                .expectedDisassembly = "TAS D1"
            },
            TASTestParams {
                .name = "Data_register_EA_with_positive_value_and_full_CCR",
                .data = 0x04,
                .ccr = 0xFF,
                .eaMode = 0,
                .eaReg = 1,
                .expectedCycles = 4,
                .expectedResultData = 0x84,
                .expectedCcr = CCR_EXTEND,
                .expectedDisassembly = "TAS D1"
            },
            TASTestParams {
                .name = "Address_register_indirect_EA_with_value_0_and_cleared_CCR",
                .data = 0,
                .ccr = 0,
                .eaMode = 2,
                .eaReg = 1,
                .expectedCycles = 12,
                .expectedResultData = 0b10000000,
                .expectedCcr = CCR_ZERO,
                .expectedDisassembly = "TAS (A1)"
            },
            TASTestParams {
                .name = "Address_register_indirect_EA_with_negative_value_and_cleared_CCR",
                .data = 0xF4,
                .ccr = 0,
                .eaMode = 2,
                .eaReg = 1,
                .expectedCycles = 12,
                .expectedResultData = 0xF4,
                .expectedCcr = CCR_NEGATIVE,
                .expectedDisassembly = "TAS (A1)"
            },
            TASTestParams {
                .name = "Address_register_indirect_EA_with_positive_value_and_cleared_CCR",
                .data = 0x04,
                .ccr = 0,
                .eaMode = 2,
                .eaReg = 1,
                .expectedCycles = 12,
                .expectedResultData = 0x84,
                .expectedCcr = 0,
                .expectedDisassembly = "TAS (A1)"
            },
            TASTestParams {
                .name = "Address_register_indirect_EA_with_positive_value_and_full_CCR",
                .data = 0x04,
                .ccr = 0xFF,
                .eaMode = 2,
                .eaReg = 1,
                .expectedCycles = 12,
                .expectedResultData = 0x84,
                .expectedCcr = CCR_EXTEND,
                .expectedDisassembly = "TAS (A1)"
            }
    ),
    [](const ::testing::TestParamInfo<TASTestParams>& info) {
        return info.param.name;
    }
);
