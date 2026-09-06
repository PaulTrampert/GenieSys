//
// Created by pault on 9/22/2021.
//
#include <gtest/gtest.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/CpuOperations/MOVE.h>
#include <GenieSys/enums.h>



struct MOVETest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    GenieSys::MOVE* subject;

    uint16_t moveByte = 0b0001000010010001; // MOVE.b (A1),(A0)
    uint16_t moveWord = 0b0011000010010001; // MOVE.w (A1),(A0)
    uint16_t moveLong = 0b0010000010010001; // MOVE.l (A1),(A0)

    MOVETest() {
        cpu = bus.getCpu();
        subject = new GenieSys::MOVE(cpu, &bus);
    }

    ~MOVETest() override {
        delete subject;
    }
public:
    void SetUp() override {
        cpu->setCcrFlags(GenieSys::CCR_EXTEND | GenieSys::CCR_NEGATIVE | GenieSys::CCR_ZERO | GenieSys::CCR_OVERFLOW | GenieSys::CCR_CARRY);
        cpu->setAddressRegister(0, 500);
        cpu->setAddressRegister(1, 600);
        bus.writeLong(500, 0);
        bus.writeLong(600, 0xAABBCCDD);
    }
};

TEST_F(MOVETest, DisassembleMoveByte) {
    ASSERT_EQ("MOVE.b (A1),(A0)", subject->disassemble(moveByte));
}

TEST_F(MOVETest, DisassembleMoveWord) {
    ASSERT_EQ("MOVE.w (A1),(A0)", subject->disassemble(moveWord));
}

TEST_F(MOVETest, DisassembleMoveLong) {
    ASSERT_EQ("MOVE.l (A1),(A0)", subject->disassemble(moveLong));
}

TEST_F(MOVETest, ExecuteMoveByte) {
    ASSERT_EQ(12, subject->execute(moveByte));
    ASSERT_EQ(0xAA000000, bus.readLong(500));
    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(MOVETest, ExecuteMoveWord) {
    ASSERT_EQ(12, subject->execute(moveWord));
    ASSERT_EQ(0xAABB0000, bus.readLong(500));
    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(MOVETest, ExecuteMoveLong) {
    ASSERT_EQ(20, subject->execute(moveLong));
    ASSERT_EQ(0xAABBCCDD, bus.readLong(500));
    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_NEGATIVE, cpu->getCcrFlags());
}

TEST_F(MOVETest, ExecuteMoveWordFromAbsoluteShort) {
    // MOVE.w (xxx).W,D0 -> 0011 000 000 111 000. Absolute short is row 7 of the cycle table,
    // not row 0, even though its mode id is 0b000.
    bus.writeWord(cpu->getPc(), 600);
    ASSERT_EQ(12, subject->execute(0b0011000000111000));
    ASSERT_EQ(0xAABB, cpu->getDataRegister(0));
}

TEST_F(MOVETest, ExecuteMoveLongFromAbsoluteShort) {
    // MOVE.l (xxx).W,D0 -> 0010 000 000 111 000
    bus.writeWord(cpu->getPc(), 600);
    ASSERT_EQ(16, subject->execute(0b0010000000111000));
    ASSERT_EQ(0xAABBCCDD, cpu->getDataRegister(0));
}

TEST_F(MOVETest, ExecuteMoveWordToAbsoluteShort) {
    // MOVE.w D1,(xxx).W -> 0011 000 111 000 001
    cpu->setDataRegister(1, (uint32_t)0x0000BEEF);
    bus.writeWord(cpu->getPc(), 500);
    ASSERT_EQ(12, subject->execute(0b0011000111000001));
    ASSERT_EQ(0xBEEF, bus.readWord(500));
}

TEST_F(MOVETest, ExecuteMoveWordToAbsoluteLong) {
    // MOVE.w D1,(xxx).L -> 0011 001 111 000 001
    cpu->setDataRegister(1, (uint32_t)0x0000BEEF);
    bus.writeLong(cpu->getPc(), 500);
    ASSERT_EQ(16, subject->execute(0b0011001111000001));
    ASSERT_EQ(0xBEEF, bus.readWord(500));
}

TEST_F(MOVETest, ExecuteWithProgramCounterDestinationTrapsAsIllegal) {
    // MOVE.w D1,d(PC) -> 0011 010 111 000 001. Mode 0b111 only owns (xxx).W and (xxx).L as a
    // destination; the remaining sub modes would index past the end of the cycle table.
    bus.writeLong(GenieSys::TV_ILLEGAL_INSTR * 4, 0x1000);
    ASSERT_EQ(34, subject->execute(0b0011010111000001));
    ASSERT_EQ(0x1000, cpu->getPc());
}

TEST_F(MOVETest, ExecuteWithImmediateDestinationTrapsAsIllegal) {
    // MOVE.w #imm,#imm -> 0011 100 111 111 100
    bus.writeLong(GenieSys::TV_ILLEGAL_INSTR * 4, 0x1000);
    ASSERT_EQ(34, subject->execute(0b0011100111111100));
    ASSERT_EQ(0x1000, cpu->getPc());
}

TEST_F(MOVETest, DisassembleWithIllegalDestination) {
    ASSERT_EQ("ILLEGAL", subject->disassemble(0b0011100111111100));
}
