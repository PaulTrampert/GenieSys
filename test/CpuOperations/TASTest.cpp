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
            TASTestParams{"Data_register_EA_with_value_0_and_cleared_CCR", 0, 0, 0, 1, 4, 0b10000000, CCR_ZERO, "TAS D1"},
            TASTestParams{"Data_register_EA_with_negative_value_and_cleared_CCR", 0xF4, 0, 0, 1, 4, 0xF4, CCR_NEGATIVE, "TAS D1"},
            TASTestParams{"Data_register_EA_with_positive_value_and_cleared_CCR", 0x04, 0, 0, 1, 4, 0x84, 0, "TAS D1"},
            TASTestParams{"Data_register_EA_with_positive_value_and_full_CCR", 0x04, 0xFF, 0, 1, 4, 0x84, CCR_EXTEND, "TAS D1"},
            TASTestParams{"Address_register_indirect_EA_with_value_0_and_cleared_CCR", 0, 0, 2, 1, 12, 0b10000000, CCR_ZERO, "TAS (A1)"},
            TASTestParams{"Address_register_indirect_EA_with_negative_value_and_cleared_CCR", 0xF4, 0, 2, 1, 12, 0xF4, CCR_NEGATIVE, "TAS (A1)"},
            TASTestParams{"Address_register_indirect_EA_with_positive_value_and_cleared_CCR", 0x04, 0, 2, 1, 12, 0x84, 0, "TAS (A1)"},
            TASTestParams{"Address_register_indirect_EA_with_positive_value_and_full_CCR", 0x04, 0xFF, 2, 1, 12, 0x84, CCR_EXTEND, "TAS (A1)"}
    ),
    [](const ::testing::TestParamInfo<TASTestParams>& info) {
        return info.param.name;
    }
);
