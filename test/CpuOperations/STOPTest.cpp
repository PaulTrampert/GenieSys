//
// Created by pault on 2/7/2022.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/STOP.h>
#include "../MockBus.h"
#include "../MockCpu.h"
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/numberUtils.h>
#include <string>

using namespace GenieSys;

struct STOPTestParam {
    bool isSupervisor;
    SR_TRACE_STATUS trace;
    uint16_t immArg;

    uint8_t expectedCycles;
    std::string expectedDisassembly;
};

struct STOPTest : testing::TestWithParam<STOPTestParam> {
public:
    uint16_t opWord = 0b0100111001110010;

    MockCpu cpu;
    MockBus bus;
    STOP *subject;

    ProgramCounterAddressingMode *immMode;

    STOPTest() {
        subject = new STOP(&cpu, &bus);
        immMode = new ProgramCounterAddressingMode(&cpu, &bus);
    }

    ~STOPTest() override {
        delete subject;
        delete immMode;
    }

protected:
    void SetUp() override {
        ON_CALL(cpu, getAddressingMode(ProgramCounterAddressingMode::MODE_ID))
                .WillByDefault(testing::Return(immMode));
    }
};

TEST_P(STOPTest, Execute) {
    auto param = GetParam();
    ON_CALL(cpu, isSupervisor())
        .WillByDefault(testing::Return(param.isSupervisor));
    ON_CALL(cpu, isTraceEnabled())
        .WillByDefault(testing::Return(param.trace));
    ON_CALL(bus, read(testing::A<uint32_t>(), (uint8_t)2))
        .WillByDefault(testing::Return(getBytes(param.immArg)));

    EXPECT_CALL(cpu, trap(TV_PRIVILEGE))
        .Times(param.isSupervisor ? 0 : 1);
    EXPECT_CALL(cpu, trap(TV_TRACE))
        .Times((param.isSupervisor && param.trace != NO_TRACE) ? 1 : 0);
    EXPECT_CALL(bus, stop())
        .Times((param.isSupervisor && param.trace == NO_TRACE) ? 1 : 0);
    EXPECT_CALL(cpu, setSR(param.immArg))
        .Times(param.isSupervisor ? 1 : 0);

    EXPECT_EQ(param.expectedCycles, subject->execute(opWord));
}

TEST_P(STOPTest, Disassemble) {
    auto param = GetParam();
    ON_CALL(bus, read(testing::A<uint32_t>(), (uint8_t)2))
            .WillByDefault(testing::Return(getBytes(param.immArg)));

    EXPECT_EQ(param.expectedDisassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_CASE_P(STOP, STOPTest,
    testing::Values(
        STOPTestParam {
            .isSupervisor =  false,
            .trace =  NO_TRACE,
            .immArg =  0,
            .expectedCycles =  0,
            .expectedDisassembly = "STOP $0000"
        },
        STOPTestParam {
            .isSupervisor =  true,
            .trace =  NO_TRACE,
            .immArg =  50,
            .expectedCycles =  4,
            .expectedDisassembly = "STOP $0032"
        },
        STOPTestParam {
            .isSupervisor =  true,
            .trace =  TRACE_CHANGE,
            .immArg =  50,
            .expectedCycles =  0,
            .expectedDisassembly = "STOP $0032"
        }
    ));