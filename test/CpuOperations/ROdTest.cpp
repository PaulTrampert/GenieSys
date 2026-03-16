//
// Created by copilot
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ROd.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>


class ROdTest : public ::testing::Test {
public:
    GenieSys::ROd* subject;
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;

    // Register form (immediate count): 1110 ccc d ss 0 11 rrr
    //   ROL.b #1,D0 = 1110 001 1 00 0 11 000 = 0xE318
    uint16_t rolByteImm = 0xE318;
    //   ROR.b #1,D0 = 1110 001 0 00 0 11 000 = 0xE218
    uint16_t rorByteImm = 0xE218;
    //   ROL.w #1,D0 = 1110 001 1 01 0 11 000 = 0xE358
    uint16_t rolWordImm = 0xE358;
    //   ROR.w #1,D0 = 1110 001 0 01 0 11 000 = 0xE258
    uint16_t rorWordImm = 0xE258;
    //   ROL.l #1,D0 = 1110 001 1 10 0 11 000 = 0xE398
    uint16_t rolLongImm = 0xE398;
    //   ROR.l #1,D0 = 1110 001 0 10 0 11 000 = 0xE298
    uint16_t rorLongImm = 0xE298;

    // Register form (immediate count=8): 1110 000 d ss 0 11 rrr (field 000 encodes 8)
    //   ROL.b #8,D0 = 1110 000 1 00 0 11 000 = 0xE118
    uint16_t rolByteImm8 = 0xE118;
    //   ROR.b #8,D0 = 1110 000 0 00 0 11 000 = 0xE018
    uint16_t rorByteImm8 = 0xE018;

    // Register form (register count from D1): 1110 001 d ss 1 11 rrr
    //   ROL.b D1,D0 = 1110 001 1 00 1 11 000 = 0xE338
    uint16_t rolByteReg = 0xE338;
    //   ROR.b D1,D0 = 1110 001 0 00 1 11 000 = 0xE238
    uint16_t rorByteReg = 0xE238;

    // Memory form: 1110 011 d 11 eaMode eaReg
    //   ROL.w (A0) = 1110 011 1 11 010 000 = 0xE7D0
    uint16_t rolWordMem = 0xE7D0;
    //   ROR.w (A0) = 1110 011 0 11 010 000 = 0xE6D0
    uint16_t rorWordMem = 0xE6D0;

    ROdTest() : Test() {
        cpu = bus.getCpu();
        subject = new GenieSys::ROd(cpu, &bus);
    }

    ~ROdTest() override {
        delete subject;
    }

protected:
    void SetUp() override {
        cpu->setDataRegister(0, (uint32_t)0);
        cpu->setDataRegister(1, (uint32_t)0);
        cpu->setAddressRegister(0, 600);
        cpu->setCcrFlags(0);
    }
};

// ===== Disassembly tests =====

TEST_F(ROdTest, DisassembleRolByteImm) {
    ASSERT_EQ("ROL.b #1,D0", subject->disassemble(rolByteImm));
}

TEST_F(ROdTest, DisassembleRorByteImm) {
    ASSERT_EQ("ROR.b #1,D0", subject->disassemble(rorByteImm));
}

TEST_F(ROdTest, DisassembleRolWordImm) {
    ASSERT_EQ("ROL.w #1,D0", subject->disassemble(rolWordImm));
}

TEST_F(ROdTest, DisassembleRorWordImm) {
    ASSERT_EQ("ROR.w #1,D0", subject->disassemble(rorWordImm));
}

TEST_F(ROdTest, DisassembleRolLongImm) {
    ASSERT_EQ("ROL.l #1,D0", subject->disassemble(rolLongImm));
}

TEST_F(ROdTest, DisassembleRorLongImm) {
    ASSERT_EQ("ROR.l #1,D0", subject->disassemble(rorLongImm));
}

TEST_F(ROdTest, DisassembleRolByteImm8) {
    ASSERT_EQ("ROL.b #8,D0", subject->disassemble(rolByteImm8));
}

TEST_F(ROdTest, DisassembleRorByteImm8) {
    ASSERT_EQ("ROR.b #8,D0", subject->disassemble(rorByteImm8));
}

TEST_F(ROdTest, DisassembleRolByteReg) {
    ASSERT_EQ("ROL.b D1,D0", subject->disassemble(rolByteReg));
}

TEST_F(ROdTest, DisassembleRorByteReg) {
    ASSERT_EQ("ROR.b D1,D0", subject->disassemble(rorByteReg));
}

TEST_F(ROdTest, DisassembleRolWordMem) {
    ASSERT_EQ("ROL.w (A0)", subject->disassemble(rolWordMem));
}

TEST_F(ROdTest, DisassembleRorWordMem) {
    ASSERT_EQ("ROR.w (A0)", subject->disassemble(rorWordMem));
}

// ===== ROL register form - immediate count =====

TEST_F(ROdTest, RolByteImmBasic) {
    // ROL.b #1,D0: 0x01 -> 0x02, C=0
    cpu->setDataRegister(0, (uint32_t)0x01);
    ASSERT_EQ(8, subject->execute(rolByteImm)); // 6 + 2*1 = 8
    ASSERT_EQ(0x02u, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolByteImmWrapsAround) {
    // ROL.b #1,D0: 0x80 -> 0x01, C=1 (MSB wraps to LSB, then C = new LSB)
    cpu->setDataRegister(0, (uint32_t)0x80);
    subject->execute(rolByteImm);
    ASSERT_EQ(0x01u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolByteImmSetsNegative) {
    // ROL.b #1,D0: 0x40 -> 0x80, N set
    cpu->setDataRegister(0, (uint32_t)0x40);
    subject->execute(rolByteImm);
    ASSERT_EQ(0x80u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolByteImmSetsZero) {
    // ROL.b #1,D0: 0x00 -> 0x00, Z set, C=0
    cpu->setDataRegister(0, (uint32_t)0x00);
    subject->execute(rolByteImm);
    ASSERT_EQ(0x00u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolByteImmPreservesUpperRegisterBits) {
    // ROL.b only affects low byte; upper 3 bytes of register should be preserved
    cpu->setDataRegister(0, (uint32_t)0xDEADBE01);
    subject->execute(rolByteImm);
    ASSERT_EQ(0xDEADBE02u, cpu->getDataRegister(0));
}

TEST_F(ROdTest, RolWordImmBasic) {
    // ROL.w #1,D0: 0x0001 -> 0x0002
    cpu->setDataRegister(0, (uint32_t)0x0001);
    ASSERT_EQ(8, subject->execute(rolWordImm)); // 6 + 2*1 = 8
    ASSERT_EQ(0x0002u, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolWordImmWrapsAround) {
    // ROL.w #1,D0: 0x8000 -> 0x0001, C=1
    cpu->setDataRegister(0, (uint32_t)0x8000);
    subject->execute(rolWordImm);
    ASSERT_EQ(0x0001u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolLongImmBasic) {
    // ROL.l #1,D0: 0x00000001 -> 0x00000002
    cpu->setDataRegister(0, (uint32_t)0x00000001);
    ASSERT_EQ(10, subject->execute(rolLongImm)); // 8 + 2*1 = 10
    ASSERT_EQ(0x00000002u, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolLongImmWrapsAround) {
    // ROL.l #1,D0: 0x80000000 -> 0x00000001, C=1
    cpu->setDataRegister(0, (uint32_t)0x80000000);
    subject->execute(rolLongImm);
    ASSERT_EQ(0x00000001u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

// ===== ROR register form - immediate count =====

TEST_F(ROdTest, RorByteImmBasic) {
    // ROR.b #1,D0: 0x02 -> 0x01, C=0
    cpu->setDataRegister(0, (uint32_t)0x02);
    ASSERT_EQ(8, subject->execute(rorByteImm)); // 6 + 2*1 = 8
    ASSERT_EQ(0x01u, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorByteImmWrapsAround) {
    // ROR.b #1,D0: 0x01 -> 0x80, C=1 (LSB wraps to MSB)
    cpu->setDataRegister(0, (uint32_t)0x01);
    subject->execute(rorByteImm);
    ASSERT_EQ(0x80u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorByteImmSetsZero) {
    // ROR.b #1,D0: 0x00 -> 0x00, Z=1, C=0
    cpu->setDataRegister(0, (uint32_t)0x00);
    subject->execute(rorByteImm);
    ASSERT_EQ(0x00u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorByteImmPreservesUpperRegisterBits) {
    cpu->setDataRegister(0, (uint32_t)0xDEADBE02);
    subject->execute(rorByteImm);
    ASSERT_EQ(0xDEADBE01u, cpu->getDataRegister(0));
}

TEST_F(ROdTest, RorWordImmBasic) {
    // ROR.w #1,D0: 0x0002 -> 0x0001
    cpu->setDataRegister(0, (uint32_t)0x0002);
    ASSERT_EQ(8, subject->execute(rorWordImm)); // 6 + 2*1 = 8
    ASSERT_EQ(0x0001u, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorWordImmWrapsAround) {
    // ROR.w #1,D0: 0x0001 -> 0x8000, C=1, N=1
    cpu->setDataRegister(0, (uint32_t)0x0001);
    subject->execute(rorWordImm);
    ASSERT_EQ(0x8000u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorLongImmBasic) {
    // ROR.l #1,D0: 0x00000002 -> 0x00000001
    cpu->setDataRegister(0, (uint32_t)0x00000002);
    ASSERT_EQ(10, subject->execute(rorLongImm)); // 8 + 2*1 = 10
    ASSERT_EQ(0x00000001u, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorLongImmWrapsAround) {
    // ROR.l #1,D0: 0x00000001 -> 0x80000000, C=1, N=1
    cpu->setDataRegister(0, (uint32_t)0x00000001);
    subject->execute(rorLongImm);
    ASSERT_EQ(0x80000000u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

// ===== Immediate count = 8 (encoded as field 000) =====

TEST_F(ROdTest, RolByteImm8) {
    // ROL.b #8,D0: full byte rotation - result equals original
    cpu->setDataRegister(0, (uint32_t)0xA5); // 1010 0101
    ASSERT_EQ(22, subject->execute(rolByteImm8)); // 6 + 2*8 = 22
    ASSERT_EQ(0xA5u, cpu->getDataRegister(0)); // same value after full rotation
    // C = bit 0 of result = 1 (LSB of 0xA5)
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorByteImm8) {
    // ROR.b #8,D0: full byte rotation - result equals original
    cpu->setDataRegister(0, (uint32_t)0xA5); // 1010 0101
    ASSERT_EQ(22, subject->execute(rorByteImm8)); // 6 + 2*8 = 22
    ASSERT_EQ(0xA5u, cpu->getDataRegister(0)); // same value after full rotation
    // C = MSB of result = 1 (bit 7 of 0xA5)
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

// ===== Register count form =====

TEST_F(ROdTest, RolByteRegCount1) {
    // ROL.b D1,D0 with D1=1: same as ROL.b #1,D0
    cpu->setDataRegister(0, (uint32_t)0x80);
    cpu->setDataRegister(1, (uint32_t)1);
    ASSERT_EQ(8, subject->execute(rolByteReg)); // 6 + 2*1 = 8
    ASSERT_EQ(0x01u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolByteRegCount0) {
    // ROL.b D1,D0 with D1=0: no rotation, result unchanged, C cleared
    cpu->setDataRegister(0, (uint32_t)0x80);
    cpu->setDataRegister(1, (uint32_t)0);
    ASSERT_EQ(6, subject->execute(rolByteReg)); // 6 + 2*0 = 6
    ASSERT_EQ(0x80u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags()); // N set, C=0
}

TEST_F(ROdTest, RorByteRegCount1) {
    // ROR.b D1,D0 with D1=1: 0x01 -> 0x80, C=1
    cpu->setDataRegister(0, (uint32_t)0x01);
    cpu->setDataRegister(1, (uint32_t)1);
    ASSERT_EQ(8, subject->execute(rorByteReg)); // 6 + 2*1 = 8
    ASSERT_EQ(0x80u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolByteRegCountModulo64) {
    // ROL.b D1,D0 with D1=64: effective count = 0, no rotation
    cpu->setDataRegister(0, (uint32_t)0x80);
    cpu->setDataRegister(1, (uint32_t)64);
    ASSERT_EQ(6, subject->execute(rolByteReg)); // 6 + 2*0 = 6
    ASSERT_EQ(0x80u, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

// ===== Extend flag not affected =====

TEST_F(ROdTest, ExtendFlagNotAffected) {
    // X flag should not be changed by ROL/ROR
    cpu->setDataRegister(0, (uint32_t)0x01);
    cpu->setCcrFlags(GenieSys::CCR_EXTEND);
    subject->execute(rolByteImm);
    // X should still be set
    ASSERT_TRUE((cpu->getCcrFlags() & GenieSys::CCR_EXTEND) != 0);
}

TEST_F(ROdTest, OverflowAlwaysCleared) {
    // V flag must always be 0
    cpu->setDataRegister(0, (uint32_t)0xFF);
    cpu->setCcrFlags(GenieSys::CCR_OVERFLOW);
    subject->execute(rolByteImm);
    ASSERT_TRUE((cpu->getCcrFlags() & GenieSys::CCR_OVERFLOW) == 0);
}

// ===== Memory form =====

TEST_F(ROdTest, RolWordMemBasic) {
    // ROL.w (A0): rotate 0x0001 left by 1 -> 0x0002, C=0
    bus.writeWord(600, 0x0001);
    ASSERT_EQ(12, subject->execute(rolWordMem)); // 8 + 4 ea cycles
    ASSERT_EQ(0x0002, bus.readWord(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolWordMemWrapsAround) {
    // ROL.w (A0): rotate 0x8000 left by 1 -> 0x0001, C=1
    bus.writeWord(600, 0x8000);
    subject->execute(rolWordMem);
    ASSERT_EQ(0x0001, bus.readWord(600));
    ASSERT_EQ(GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROdTest, RolWordMemSetsNegative) {
    // ROL.w (A0): rotate 0x4000 left by 1 -> 0x8000, N=1
    bus.writeWord(600, 0x4000);
    subject->execute(rolWordMem);
    ASSERT_EQ(0x8000, bus.readWord(600));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorWordMemBasic) {
    // ROR.w (A0): rotate 0x0002 right by 1 -> 0x0001, C=0
    bus.writeWord(600, 0x0002);
    ASSERT_EQ(12, subject->execute(rorWordMem)); // 8 + 4 ea cycles
    ASSERT_EQ(0x0001, bus.readWord(600));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorWordMemWrapsAround) {
    // ROR.w (A0): rotate 0x0001 right by 1 -> 0x8000, C=1, N=1
    bus.writeWord(600, 0x0001);
    subject->execute(rorWordMem);
    ASSERT_EQ(0x8000, bus.readWord(600));
    ASSERT_EQ(GenieSys::CCR_NEGATIVE | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ROdTest, RorWordMemSetsZero) {
    // ROR.w (A0): rotate 0x0000 right by 1 -> 0x0000, Z=1, C=0
    bus.writeWord(600, 0x0000);
    subject->execute(rorWordMem);
    ASSERT_EQ(0x0000, bus.readWord(600));
    ASSERT_EQ(GenieSys::CCR_ZERO, cpu->getCcrFlags());
}
