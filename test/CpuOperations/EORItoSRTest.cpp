//
// Created by paul.trampert on 12/25/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/EORItoSR.h>



struct EORItoSRTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    EORItoSR* subject;

    EORItoSRTest() {
        cpu = bus.getCpu();
        subject = new EORItoSR(cpu, &bus);
    }

    ~EORItoSRTest() override {
        delete subject;
    }
};

TEST_F(EORItoSRTest, ItHasTheCorrectSpecificity) {
    ASSERT_EQ(0, subject->getSpecificity());
}

TEST_F(EORItoSRTest, ItHasTheCorrectOpcodes) {
    ASSERT_EQ(1, subject->getOpcodes().size());
    ASSERT_EQ(0b0000101001111100, subject->getOpcodes()[0]);
}

TEST_F(EORItoSRTest, ItDisassemblesCorrectly) {
    bus.writeWord(10, 0xFFFF);
    cpu->setPc(10);
    ASSERT_EQ("EORI $ffff,SR", subject->disassemble(0b0000101001111100));
}

TEST_F(EORItoSRTest, ItCorrectlyUpdatesTheSR_WhenInSupervisorMode) {
    bus.writeWord(10, 0b0011111111111111);
    cpu->setPc(10);
    cpu->setSR(0b1110000000000000);

    subject->execute(0b0000101001111100);

    ASSERT_EQ(0b1101111111111111, cpu->getSR());
}

TEST_F(EORItoSRTest, ItTraps_WhenNotInSupervisorMode) {
    bus.writeWord(10, 0b1111111111111111);
    bus.writeLong(0x20, 0xABCD1234);
    cpu->setPc(10);
    cpu->setSR(0);

    subject->execute(0b0000101001111100);

    ASSERT_EQ(8192, cpu->getSR());
    ASSERT_EQ(0xABCD1234, cpu->getPc());
}

TEST_F(EORItoSRTest, ItReturnsTheCorrectNumberOfClockCycles) {
    ASSERT_EQ(20, subject->execute(0b0000101001111100));
}