//
// Created by pault on 9/22/2021.
//
#include <gtest/gtest.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>
#include <GenieSys/CpuOperations/MOVE.h>



struct MOVETest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    MOVE* subject;

    uint16_t moveByte = 0b0001000010010001; // MOVE.b (A1),(A0)
    uint16_t moveWord = 0b0011000010010001; // MOVE.w (A1),(A0)
    uint16_t moveLong = 0b0010000010010001; // MOVE.l (A1),(A0)

    MOVETest() {
        cpu = bus.getCpu();
        subject = new MOVE(cpu, &bus);
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