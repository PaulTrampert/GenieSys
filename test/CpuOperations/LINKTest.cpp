//
// Created by paul on 12/11/21.
//

#include <GenieSys/CpuOperations/LINK.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <gtest/gtest.h>
#include <string>

using namespace GenieSys;

struct LINKTestParams {
    uint8_t addrReg;
    uint16_t displacement;
    uint32_t addr;
    uint32_t sp;
    uint32_t pc;
    std::string disassembly;
};

class LINKTest : public testing::TestWithParam<LINKTestParams> {
public:
    uint16_t baseOpWord = 0b0100111001010000;
    BitMask<uint16_t> regMask = BitMask<uint16_t>(2, 3);
    Bus bus;
    M68kCpu * cpu;
    LINK * subject;

    LINKTest() : TestWithParam() {
        cpu = bus.getCpu();
        subject = new LINK(cpu, &bus);
    }

    ~LINKTest() {
        delete subject;
    }
};

TEST_P(LINKTest, Execute) {
    auto params = GetParam();
    cpu->setAddressRegister(params.addrReg, params.addr);
    cpu->setPc(params.pc);
    cpu->setStackPointer(params.sp);
    bus.writeWord(params.pc, params.displacement);
    uint16_t opWord = regMask.compose(baseOpWord, params.addrReg);

    uint8_t cycles = subject->execute(opWord);

    ASSERT_EQ(16, cycles);
    ASSERT_EQ(params.pc + 2, cpu->getPc());
    ASSERT_EQ(params.sp - 4 + params.displacement, cpu->getStackPointer());
    ASSERT_EQ(cpu->getStackPointer() - params.displacement, cpu->getAddressRegister(params.addrReg));
}

TEST_P(LINKTest, Disassemble) {
    auto params = GetParam();
    cpu->setAddressRegister(params.addrReg, params.addr);
    cpu->setPc(params.pc);
    cpu->setStackPointer(params.sp);
    bus.writeWord(params.pc, params.displacement);
    uint16_t opWord = regMask.compose(baseOpWord, params.addrReg);

    ASSERT_EQ(params.disassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_CASE_P(LINK, LINKTest,
    testing::Values(
        LINKTestParams {
            .addrReg = 2,
            .displacement = static_cast<uint16_t>(-16),
            .addr = 0xAABBCCDD,
            .sp = 6000,
            .pc = 128,
            .disassembly = "LINK A2, $fff0"
        }
    )
);