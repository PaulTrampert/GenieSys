//
// Created by paul on 11/24/25.
//

#include <GenieSys/CpuOperations/CHK.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <gtest/gtest.h>
#include <string>

using namespace GenieSys;

struct CHKTestParams {
    uint8_t dataReg;
    uint8_t eaMode;
    uint8_t eaAddr;
    int16_t dnValue;
    int16_t eaValue;
    uint32_t pc;
    uint32_t eaAddress;
    uint8_t expectedCycles;
    uint8_t expectedCcrFlags;
    bool expectTrap;
    std::string disassembly;
};

class CHKTest : public testing::TestWithParam<CHKTestParams> {
public:
    uint16_t baseOpWord = 0b0100000000000000;
    BitMask<uint16_t> dnMask = BitMask<uint16_t>(11, 3);
    BitMask<uint16_t> sizeMask = BitMask<uint16_t>(8, 2, 3, 3);
    BitMask<uint16_t> eaModeMask = BitMask<uint16_t>(5, 3);
    BitMask<uint16_t> eaAddrMask = BitMask<uint16_t>(2, 3);
    Bus bus;
    M68kCpu * cpu;
    CHK * subject;

    CHKTest() : TestWithParam() {
        cpu = bus.getCpu();
        subject = new CHK(cpu, &bus);
    }

    ~CHKTest() override {
        delete subject;
    }
};

TEST_P(CHKTest, Execute) {
    const auto& params = GetParam();

    // Set up the data register value
    cpu->setDataRegister(params.dataReg, static_cast<uint16_t>(params.dnValue));

    // Set up the effective address value
    if (params.eaMode == 0) {
        // Data register direct mode
        cpu->setDataRegister(params.eaAddr, static_cast<uint16_t>(params.eaValue));
    } else if (params.eaMode == 2) {
        // Address register indirect mode
        cpu->setAddressRegister(params.eaAddr, params.eaAddress);
        bus.writeWord(params.eaAddress, static_cast<uint16_t>(params.eaValue));
    } else if (params.eaMode == 7 && params.eaAddr == 0) {
        // Absolute short addressing mode
        bus.writeWord(params.pc, static_cast<uint16_t>(params.eaAddress));
        bus.writeWord(params.eaAddress, static_cast<uint16_t>(params.eaValue));
    } else if (params.eaMode == 7 && params.eaAddr == 4) {
        // Immediate data mode
        bus.writeWord(params.pc, static_cast<uint16_t>(params.eaValue));
    }

    cpu->setPc(params.pc);

    // Set up trap vector for TV_CHK (vector 6)
    if (params.expectTrap) {
        bus.writeLong(4 * TV_CHK, 0x1000);
    }

    uint16_t opWord = dnMask.compose(baseOpWord, params.dataReg);
    opWord = sizeMask.compose(opWord, 3); // Set size field to 3 for word operation
    opWord = eaModeMask.compose(opWord, params.eaMode);
    opWord = eaAddrMask.compose(opWord, params.eaAddr);

    uint8_t cycles = subject->execute(opWord);

    ASSERT_EQ(params.expectedCycles, cycles);

    if (params.expectTrap) {
        // When trap is called, CCR flags should be set and PC should point to trap vector
        ASSERT_EQ(params.expectedCcrFlags, cpu->getCcrFlags() & CCR_NEGATIVE);
        ASSERT_EQ(0x1000, cpu->getPc());
    } else {
        // No trap, just return 10 cycles
        ASSERT_EQ(params.pc, cpu->getPc());
    }
}

TEST_P(CHKTest, Disassemble) {
    const auto& params = GetParam();

    if (params.eaMode == 7 && params.eaAddr == 0) {
        // Absolute short addressing mode
        bus.writeWord(params.pc, static_cast<uint16_t>(params.eaAddress));
    } else if (params.eaMode == 7 && params.eaAddr == 4) {
        // Immediate data mode
        bus.writeWord(params.pc, static_cast<uint16_t>(params.eaValue));
    }

    cpu->setPc(params.pc);

    uint16_t opWord = dnMask.compose(baseOpWord, params.dataReg);
    opWord = sizeMask.compose(opWord, 3); // Set size field to 3 for word operation
    opWord = eaModeMask.compose(opWord, params.eaMode);
    opWord = eaAddrMask.compose(opWord, params.eaAddr);

    ASSERT_EQ(params.disassembly, subject->disassemble(opWord));
}

INSTANTIATE_TEST_CASE_P(CHK, CHKTest,
    testing::Values(
        // Test case 1: dn < 0, should set CCR_NEGATIVE and trap
        CHKTestParams {
            .dataReg = 2,
            .eaMode = 0,  // Data register direct
            .eaAddr = 3,
            .dnValue = -5,
            .eaValue = 100,
            .pc = 128,
            .eaAddress = 0,
            .expectedCycles = 34,  // trap returns 34
            .expectedCcrFlags = CCR_NEGATIVE,
            .expectTrap = true,
            .disassembly = "CHK D3, D2"
        },
        // Test case 2: dn > ea, should clear CCR_NEGATIVE and trap
        CHKTestParams {
            .dataReg = 1,
            .eaMode = 0,  // Data register direct
            .eaAddr = 4,
            .dnValue = 150,
            .eaValue = 100,
            .pc = 256,
            .eaAddress = 0,
            .expectedCycles = 34,  // trap returns 34
            .expectedCcrFlags = 0,
            .expectTrap = true,
            .disassembly = "CHK D4, D1"
        },
        // Test case 3: 0 < dn < ea, should return 10 (no trap)
        CHKTestParams {
            .dataReg = 5,
            .eaMode = 0,  // Data register direct
            .eaAddr = 6,
            .dnValue = 50,
            .eaValue = 100,
            .pc = 512,
            .eaAddress = 0,
            .expectedCycles = 10,
            .expectedCcrFlags = 0,
            .expectTrap = false,
            .disassembly = "CHK D6, D5"
        },
        // Test case 4: dn = 0, should return 10 (no trap)
        CHKTestParams {
            .dataReg = 0,
            .eaMode = 0,  // Data register direct
            .eaAddr = 1,
            .dnValue = 0,
            .eaValue = 100,
            .pc = 768,
            .eaAddress = 0,
            .expectedCycles = 10,
            .expectedCcrFlags = 0,
            .expectTrap = false,
            .disassembly = "CHK D1, D0"
        },
        // Test case 5: dn = ea, should return 10 (no trap)
        CHKTestParams {
            .dataReg = 3,
            .eaMode = 0,  // Data register direct
            .eaAddr = 2,
            .dnValue = 100,
            .eaValue = 100,
            .pc = 1024,
            .eaAddress = 0,
            .expectedCycles = 10,
            .expectedCcrFlags = 0,
            .expectTrap = false,
            .disassembly = "CHK D2, D3"
        },
        // Test case 6: Address register indirect mode with dn < 0
        CHKTestParams {
            .dataReg = 4,
            .eaMode = 2,  // Address register indirect
            .eaAddr = 5,
            .dnValue = -10,
            .eaValue = 50,
            .pc = 1280,
            .eaAddress = 2000,
            .expectedCycles = 34,
            .expectedCcrFlags = CCR_NEGATIVE,
            .expectTrap = true,
            .disassembly = "CHK (A5), D4"
        },
        // Test case 7: Address register indirect mode with dn > ea
        CHKTestParams {
            .dataReg = 7,
            .eaMode = 2,  // Address register indirect
            .eaAddr = 0,
            .dnValue = 200,
            .eaValue = 100,
            .pc = 1536,
            .eaAddress = 3000,
            .expectedCycles = 34,
            .expectedCcrFlags = 0,
            .expectTrap = true,
            .disassembly = "CHK (A0), D7"
        },
        // Test case 8: Address register indirect mode with valid range
        CHKTestParams {
            .dataReg = 2,
            .eaMode = 2,  // Address register indirect
            .eaAddr = 3,
            .dnValue = 75,
            .eaValue = 100,
            .pc = 1792,
            .eaAddress = 4000,
            .expectedCycles = 10,
            .expectedCcrFlags = 0,
            .expectTrap = false,
            .disassembly = "CHK (A3), D2"
        }
    )
);

