//
// Created by pault on 9/17/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/MOVEP.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>



struct MOVEPTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    GenieSys::MOVEP* subject;

    uint16_t longMemToReg = 0b0000001101001001;
    uint16_t longRegToMem = 0b0000001111001001;
    uint16_t wordMemToReg = 0b0000001100001001;
    uint16_t wordRegToMem = 0b0000001110001001;

    MOVEPTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::MOVEP(cpu, &bus);
    }

    ~MOVEPTest() override {
        delete subject;
    }

public:
    void SetUp() override {
        cpu->setAddressRegister(1, 100);
        cpu->setPc(20);
        bus.writeWord(20, 16);
    }
};

TEST_F(MOVEPTest, DisassembleLongMemToReg) {
    ASSERT_EQ("MOVEP.l (16,A1),D1", subject->disassemble(longMemToReg));
}

TEST_F(MOVEPTest, DisassembleLongRegToMem) {
    ASSERT_EQ("MOVEP.l D1,(16,A1)", subject->disassemble(longRegToMem));
}

TEST_F(MOVEPTest, DisassembleWordMemToReg) {
    ASSERT_EQ("MOVEP.w (16,A1),D1", subject->disassemble(wordMemToReg));
}

TEST_F(MOVEPTest, DisassembleWordRegToMem) {
    ASSERT_EQ("MOVEP.w D1,(16,A1)", subject->disassemble(wordRegToMem));
}

TEST_F(MOVEPTest, ExecuteLongMemToReg) {
    bus.writeLong(116, 0xFF00FF00);
    bus.writeLong(120, 0xFF00FF00);
    ASSERT_EQ(24, subject->execute(longMemToReg));
    ASSERT_EQ(0xFFFFFFFF, cpu->getDataRegister(1));
}

TEST_F(MOVEPTest, ExecuteLongRegToMem) {
    cpu->setDataRegister(1, 0xFFFFFFFF);
    ASSERT_EQ(24, subject->execute(longRegToMem));
    ASSERT_EQ(0xFF00FF00, bus.readLong(116));
    ASSERT_EQ(0xFF00FF00, bus.readLong(120));

}

TEST_F(MOVEPTest, ExecuteWordMemToReg) {
    bus.writeLong(116, 0xFF00FF00);
    bus.writeLong(120, 0xFF00FF00);
    ASSERT_EQ(16, subject->execute(wordMemToReg));
    ASSERT_EQ(0x0000FFFF, cpu->getDataRegister(1));
}

TEST_F(MOVEPTest, ExecuteWordRegToMem) {
    cpu->setDataRegister(1, 0xFFFFFFFF);
    ASSERT_EQ(16, subject->execute(wordRegToMem));
    ASSERT_EQ(0xFF00FF00, bus.readLong(116));
    ASSERT_EQ(0, bus.readLong(120));
}