//
// Created by paul.trampert on 12/25/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ORItoSR.h>



struct ORItoSRTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    ORItoSR* subject;

    ORItoSRTest() {
        cpu = bus.getCpu();
        subject = new ORItoSR(cpu, &bus);
    }

    ~ORItoSRTest() override {
        delete subject;
    }
};

TEST_F(ORItoSRTest, ItHasTheCorrectSpecificity) {
    ASSERT_EQ(0, subject->getSpecificity());
}

TEST_F(ORItoSRTest, ItHasTheCorrectOpcodes) {
    ASSERT_EQ(1, subject->getOpcodes().size());
    ASSERT_EQ(0b0000000001111100, subject->getOpcodes()[0]);
}

TEST_F(ORItoSRTest, ItDisassemblesCorrectly) {
    bus.writeWord(10, 0xFFFF);
    cpu->setPc(10);
    ASSERT_EQ("ORI $ffff,SR", subject->disassemble(0b0000000001111100));
}

TEST_F(ORItoSRTest, ItCorrectlyUpdatesTheSR_WhenInSupervisorMode) {
    bus.writeWord(10, 0b1111111111111111);
    cpu->setPc(10);
    cpu->setSR(0b0010000000000000);

    subject->execute(0b0000000000111100);

    ASSERT_EQ(UINT16_MAX, cpu->getSR());
}

TEST_F(ORItoSRTest, ItTraps_WhenNotInSupervisorMode) {
    bus.writeWord(10, 0b1111111111111111);
    bus.writeLong(0x20, 0xABCD1234);
    cpu->setPc(10);
    cpu->setSR(0);

    subject->execute(0b0000000000111100);

    ASSERT_EQ(8192, cpu->getSR());
    ASSERT_EQ(0xABCD1234, cpu->getPc());
}

TEST_F(ORItoSRTest, ItReturnsTheCorrectNumberOfClockCycles) {
    ASSERT_EQ(20, subject->execute(0b0000000000111100));
}