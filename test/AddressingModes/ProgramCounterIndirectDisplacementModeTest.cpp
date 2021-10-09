//
// Created by paul.trampert on 12/3/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h>
#include <GenieSys/numberUtils.h>

struct ProgramCounterIndirectDisplacementModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    ProgramCounterIndirectDisplacementMode* subject;

    ProgramCounterIndirectDisplacementModeTest() {
        cpu = bus.getCpu();
        cpu->setPc(0x1111);
        subject = new ProgramCounterIndirectDisplacementMode(cpu, &bus);
        bus.writeWord(0x1111, 0x1111);
        bus.writeWord(0x2222, 0x3344);
    }

    ~ProgramCounterIndirectDisplacementModeTest() override {
        delete subject;
    }
};

TEST_F(ProgramCounterIndirectDisplacementModeTest, ItGetsTheCorrectMode) {
    ASSERT_EQ(0b010u, subject->getModeId());
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, TheAddressIsTheCpusPcPlusTheDisplacementWord) {
    ASSERT_EQ(0x2222, subject->getAddress(0b010u));
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, GetData_GetsTheCorrectData) {
    ASSERT_EQ(0x3344, GenieSys::bytesToWord(subject->getData(0b010u, 2)->getData()));
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, GetData_IncrementsTheProgramCounter) {
    subject->getData(0b010u, 2);
    ASSERT_EQ(0x1113, cpu->getPc());
}

TEST_F(ProgramCounterIndirectDisplacementModeTest, Disassemble) {
    ASSERT_EQ("(#4369,PC)", subject->disassemble(0b010u, 2));
}