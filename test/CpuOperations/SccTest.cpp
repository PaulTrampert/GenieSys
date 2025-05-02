//
// Created by paul on 5/2/25.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/Scc.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/ConditionCodes.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/ConditionCodes.h>
#include "GenieSys/AddressingModes/DataRegisterDirectMode.h"
#include "GenieSys/AddressingModes/AddressRegisterIndirectMode.h"

using namespace GenieSys;

struct SccTestParams {
    std::string testName;
    uint8_t condition;
    uint8_t eaMode;
    uint8_t eaReg;
    uint8_t data;
    uint8_t expectedCycles;
    uint8_t expectedData;
    std::string expectedDisassembly;
};

struct SccTest : testing::TestWithParam<SccTestParams> {
    M68kCpu* cpu;
    Bus* bus;
    Scc* subject;
    uint16_t baseOpcode = 0b0101000011000000;

    SccTest() {
        cpu = new M68kCpu();
        bus = new Bus(cpu);
        subject = new Scc(cpu, bus);
    }

    ~SccTest() override {
        delete subject;
        delete bus;
        delete cpu;
    }
};

TEST_P(SccTest, Execute) {
    SccTestParams params = GetParam();
    uint16_t opWord = baseOpcode | ((params.condition << 8) & 0b0000111100000000) | ((params.eaMode << 3) & 0b0000000000111000) | (params.eaReg);
    auto addressingMode = cpu->getAddressingMode(params.eaMode);
    auto addressingResult = addressingMode->getData(params.eaReg, 1);
    addressingResult->write(params.data);

    ASSERT_EQ(params.expectedCycles, subject->execute(opWord));

    addressingResult.reset(nullptr);
    addressingResult = addressingMode->getData(params.eaReg, 1);

    ASSERT_EQ(params.expectedData, addressingResult->getDataAsByte());
}

TEST_P(SccTest, Disassemble) {
    SccTestParams params = GetParam();
    uint16_t opWord = baseOpcode | ((params.condition << 8) & 0b0000111100000000) | ((params.eaMode << 3) & 0b0000000000111000) | (params.eaReg);

    ASSERT_EQ(params.expectedDisassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_SUITE_P(Scc, SccTest, testing::Values(
        SccTestParams {
            .testName = "Scc_WhenConditionTrue",
            .condition = CC_T,
            .eaMode = DataRegisterDirectMode::MODE_ID,
            .eaReg = 0,
            .data = 0xAA,
            .expectedCycles = 6,
            .expectedData = 0xFF,
            .expectedDisassembly = "ST D0"
        },
        SccTestParams {
            .testName = "Scc_WhenConditionFalse",
            .condition = CC_F,
            .eaMode = DataRegisterDirectMode::MODE_ID,
            .eaReg = 0,
            .data = 0xAA,
            .expectedCycles = 4,
            .expectedData = 0x00,
            .expectedDisassembly = "SF D0"
        },
        SccTestParams {
            .testName = "Scc_WhenConditionTrue_AndAddressingModeIsAddressRegisterIndirect",
            .condition = CC_T,
            .eaMode = AddressRegisterIndirectMode::MODE_ID,
            .eaReg = 0,
            .data = 0xAA,
            .expectedCycles = 12,
            .expectedData = 0xFF,
            .expectedDisassembly = "ST (A0)"
        },
        SccTestParams {
            .testName = "Scc_WhenConditionFalse_AndAddressingModeIsAddressRegisterIndirect",
            .condition = CC_F,
            .eaMode = AddressRegisterIndirectMode::MODE_ID,
            .eaReg = 0,
            .data = 0xAA,
            .expectedCycles = 12,
            .expectedData = 0x00,
            .expectedDisassembly = "SF (A0)"
        }
    ),
    [](const testing::TestParamInfo<SccTestParams>& info) {
        return info.param.testName;
    }
);
