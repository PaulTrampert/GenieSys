//
// Created by pault on 9/17/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/MOVEA.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/Bus.h>



struct MOVEATest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    GenieSys::MOVEA* subject;

    uint16_t moveWord = 0b0011001001010000;
    uint16_t moveLong = 0b0010001001010000;

    MOVEATest() {
        cpu = bus.getCpu();
        subject = new GenieSys::MOVEA(cpu, &bus);
    }

    ~MOVEATest() override {
        delete subject;
    }
public:
    void SetUp() override {
        cpu->setAddressRegister(0, 500);
        bus.writeLong(500, 0xFFF7AAAA);
    }
};

TEST_F(MOVEATest, DisassembleMoveaLong) {
    ASSERT_EQ("MOVEA.l (A0),A1", subject->disassemble(moveLong));
}

TEST_F(MOVEATest, DisassembleMoveaWord) {
    ASSERT_EQ("MOVEA.w (A0),A1", subject->disassemble(moveWord));
}

TEST_F(MOVEATest, ExecuteMoveaLong) {
    ASSERT_EQ(12, subject->execute(moveLong));
    ASSERT_EQ(0xFFF7AAAA, cpu->getAddressRegister(1));
}

TEST_F(MOVEATest, ExecuteMoveaWord) {
    ASSERT_EQ(8, subject->execute(moveWord));
    ASSERT_EQ(0xFFFFFFF7, cpu->getAddressRegister(1));
}