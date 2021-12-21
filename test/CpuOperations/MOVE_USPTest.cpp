//
// Created by pault on 12/20/2021.
//

#include <GenieSys/CpuOperations/MOVE_USP.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <gtest/gtest.h>

using namespace GenieSys;

struct MOVE_USPTestParams {
    std::string name;
    uint8_t isSuper;
    uint8_t dir;
    uint8_t reg;
    uint32_t initUsp;
    uint32_t initAddrReg;
    uint32_t expectedUsp;
    uint32_t expectedAddrReg;
    uint8_t expectedCycles;
    uint32_t expectedPc;
    std::string expectedDisassembly;
};

class MOVE_USPTest : public ::testing::TestWithParam<MOVE_USPTestParams> {
public:
    uint16_t baseOpWord = 0b0100111001100000;
    BitMask<uint16_t> dirMask = BitMask<uint16_t>(3, 1);
    BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);

    Bus bus;
    M68kCpu * cpu;
    MOVE_USP * subject;

    MOVE_USPTest() : TestWithParam() {
        cpu = bus.getCpu();
        subject = new MOVE_USP(cpu, &bus);
    }

    ~MOVE_USPTest() override {
        delete subject;
    }
};

TEST_P(MOVE_USPTest, Execute) {
    auto params = GetParam();
    uint16_t opWord = regMask.compose(dirMask.compose(baseOpWord, params.dir), params.reg);
    cpu->setSupervisor(params.isSuper);
    cpu->setAddressRegister(params.reg, params.initAddrReg);
    cpu->setUserStackPointer(params.initUsp);
    bus.writeLong(TV_PRIVILEGE * 4, 900);

    uint8_t cycles = subject->execute(opWord);

    ASSERT_EQ(params.expectedCycles, cycles);
    ASSERT_EQ(params.expectedUsp, cpu->getUserStackPointer());
    ASSERT_EQ(params.expectedAddrReg, cpu->getAddressRegister(params.reg));
    ASSERT_EQ(params.expectedPc, cpu->getPc());
}

TEST_P(MOVE_USPTest, Disassemble) {
    auto params = GetParam();
    uint16_t opWord = regMask.compose(dirMask.compose(baseOpWord, params.dir), params.reg);

    auto result = subject->disassemble(opWord);

    ASSERT_EQ(params.expectedDisassembly, result);
}

INSTANTIATE_TEST_SUITE_P(MOVE_USP, MOVE_USPTest, testing::Values(
    MOVE_USPTestParams {
        .name = "Supervisor_Move_USP_to_A3",
        .isSuper = 1,
        .dir = 1,
        .reg = 3,
        .initUsp = 400,
        .initAddrReg = 0,
        .expectedUsp = 400,
        .expectedAddrReg = 400,
        .expectedCycles = 4,
        .expectedPc = 0,
        .expectedDisassembly = "MOVE USP, A3"
    },
    MOVE_USPTestParams {
        .name = "Supervisor_Move_A3_to_USP",
        .isSuper = 1,
        .dir = 0,
        .reg = 3,
        .initUsp = 400,
        .initAddrReg = 0,
        .expectedUsp = 0,
        .expectedAddrReg = 0,
        .expectedCycles = 4,
        .expectedPc = 0,
        .expectedDisassembly = "MOVE A3, USP"
    },
    MOVE_USPTestParams {
        .name = "User_Move_USP_to_A3",
        .isSuper = 0,
        .dir = 1,
        .reg = 3,
        .initUsp = 400,
        .initAddrReg = 0,
        .expectedUsp = 400,
        .expectedAddrReg = 0,
        .expectedCycles = 34,
        .expectedPc = 900,
        .expectedDisassembly = "MOVE USP, A3"
    },
    MOVE_USPTestParams {
        .name = "User_Move_A3_to_USP",
        .isSuper = 0,
        .dir = 0,
        .reg = 3,
        .initUsp = 400,
        .initAddrReg = 0,
        .expectedUsp = 400,
        .expectedAddrReg = 0,
        .expectedCycles = 34,
        .expectedPc = 900,
        .expectedDisassembly = "MOVE A3, USP"
    }
),
[](const ::testing::TestParamInfo<MOVE_USPTestParams>& info) {
    return info.param.name;
});