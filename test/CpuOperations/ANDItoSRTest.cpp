//
// Created by paul.trampert on 12/25/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ANDItoSR.h>
#include <GenieSys/Bus.h>


struct ANDItoSRTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    ANDItoSR* subject;

    ANDItoSRTest() {
        cpu = bus.getCpu();
        subject = new ANDItoSR(cpu, &bus);
    }

    ~ANDItoSRTest() override {
        delete subject;
    }
};

TEST_F(ANDItoSRTest, ItHasTheCorrectSpecificity) {
    ASSERT_EQ(0, subject->getSpecificity());
}

TEST_F(ANDItoSRTest, ItHasTheCorrectOpcodes) {
    ASSERT_EQ(1, subject->getOpcodes().size());
    ASSERT_EQ(0b0000001001111100, subject->getOpcodes()[0]);
}

TEST_F(ANDItoSRTest, ItDisassemblesCorrectly) {
    bus.writeWord(10, 0xFFFF);
    cpu->setPc(10);
    ASSERT_EQ("ANDI $ffff,SR", subject->disassemble(0b0000000011111100));
}

TEST_F(ANDItoSRTest, ItCorrectlyUpdatesTheSR_WhenInSupervisorMode) {
    bus.writeWord(10, 0b0011111111111111);
    cpu->setPc(10);
    cpu->setSR(0b1110000000000000);

    subject->execute(0b0000001001111100);

    ASSERT_EQ(0b0010000000000000, cpu->getSR());
}

TEST_F(ANDItoSRTest, ItTraps_WhenNotInSupervisorMode) {
    bus.writeWord(10, 0b1111111111111111);
    bus.writeLong(0x20, 0xABCD1234);
    cpu->setPc(10);
    cpu->setSR(0);

    subject->execute(0b0000001001111100);

    ASSERT_EQ(8192, cpu->getSR());
    ASSERT_EQ(0xABCD1234, cpu->getPc());
}

TEST_F(ANDItoSRTest, ItReturnsTheCorrectNumberOfClockCycles) {
    ASSERT_EQ(20, subject->execute(0b0000000000111100));
}