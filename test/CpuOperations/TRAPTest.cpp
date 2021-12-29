#include <GenieSys/CpuOperations/TRAP.h>
#include <gtest/gtest.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/enums.h>

using namespace GenieSys;

struct TRAPTestParams {
    std::string name;
    TRAP_VECTORS vector;
    std::string expectedDisassembly;
};

class TRAPTest : public ::testing::TestWithParam<TRAPTestParams> {
public:
    TRAP* subject;
    Bus bus;
    M68kCpu* cpu;

    uint16_t baseOpWord = 0b0100111001000000;
    BitMask<uint16_t> vectorMask = BitMask<uint16_t>(3, 4);

    TRAPTest() : TestWithParam() {
        cpu = bus.getCpu();
        subject = new TRAP(cpu, &bus);
    }

    ~TRAPTest() override {
        delete subject;
    }
};

TEST_P(TRAPTest, Execute) {
    TRAPTestParams params = GetParam();
    ASSERT_FALSE(cpu->isSupervisor());
    bus.writeLong(4 * params.vector, 0x11223344);
    uint8_t opWord = vectorMask.compose(baseOpWord, params.vector - 32);
    ASSERT_EQ(34, subject->execute(opWord));
    ASSERT_TRUE(cpu->isSupervisor());
    ASSERT_EQ(0x11223344, cpu->getPc());
}

TEST_P(TRAPTest, Disassemble) {
    TRAPTestParams params = GetParam();
    uint8_t opWord = vectorMask.compose(baseOpWord, params.vector);
    ASSERT_EQ(params.expectedDisassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_SUITE_P(TRAP, TRAPTest,
    testing::Values(
        TRAPTestParams {
                .name = "TRAP_32",
                .vector = TV_TRAP0,
                .expectedDisassembly = "TRAP #32"
        },
        TRAPTestParams {
                .name = "TRAP_33",
                .vector = TV_TRAP1,
                .expectedDisassembly = "TRAP #33"
        },
        TRAPTestParams {
                .name = "TRAP_34",
                .vector = TV_TRAP2,
                .expectedDisassembly = "TRAP #34"
        },
        TRAPTestParams {
                .name = "TRAP_35",
                .vector = TV_TRAP3,
                .expectedDisassembly = "TRAP #35"
        },
        TRAPTestParams {
                .name = "TRAP_36",
                .vector = TV_TRAP4,
                .expectedDisassembly = "TRAP #36"
        },
        TRAPTestParams {
                .name = "TRAP_37",
                .vector = TV_TRAP5,
                .expectedDisassembly = "TRAP #37"
        },
        TRAPTestParams {
                .name = "TRAP_38",
                .vector = TV_TRAP6,
                .expectedDisassembly = "TRAP #38"
        },
        TRAPTestParams {
                .name = "TRAP_39",
                .vector = TV_TRAP7,
                .expectedDisassembly = "TRAP #39"
        },
        TRAPTestParams {
                .name = "TRAP_40",
                .vector = TV_TRAP8,
                .expectedDisassembly = "TRAP #40"
        },
        TRAPTestParams {
                .name = "TRAP_41",
                .vector = TV_TRAP9,
                .expectedDisassembly = "TRAP #41"
        },
        TRAPTestParams {
                .name = "TRAP_42",
                .vector = TV_TRAP10,
                .expectedDisassembly = "TRAP #42"
        },
        TRAPTestParams {
                .name = "TRAP_43",
                .vector = TV_TRAP11,
                .expectedDisassembly = "TRAP #43"
        },
        TRAPTestParams {
                .name = "TRAP_44",
                .vector = TV_TRAP12,
                .expectedDisassembly = "TRAP #44"
        },
        TRAPTestParams {
                .name = "TRAP_45",
                .vector = TV_TRAP13,
                .expectedDisassembly = "TRAP #45"
        },
        TRAPTestParams {
                .name = "TRAP_46",
                .vector = TV_TRAP14,
                .expectedDisassembly = "TRAP #46"
        },
        TRAPTestParams {
                .name = "TRAP_47",
                .vector = TV_TRAP15,
                .expectedDisassembly = "TRAP #47"
        }
    ),
    [](const ::testing::TestParamInfo<TRAPTestParams>& info) {
        return info.param.name;
    }
);