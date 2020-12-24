//
// Created by paul.trampert on 12/16/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ORItoCCR.h>

struct ORItoCCRTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    ORItoCCR* subject;

    ORItoCCRTest() {
        cpu = bus.getCpu();
        subject = new ORItoCCR(cpu, &bus);
    }

    ~ORItoCCRTest() override {
        delete subject;
    }
};

TEST_F(ORItoCCRTest, ItHasTheCorrectSpecificity) {
    ASSERT_EQ(0, subject->getSpecificity());
}

TEST_F(ORItoCCRTest, ItHasTheCorrectOpcodes) {
    ASSERT_EQ(1, subject->getOpcodes().size());
    ASSERT_EQ(0b0000000000111100, subject->getOpcodes()[0]);
}

TEST_F(ORItoCCRTest, ItDisassemblesCorrectly) {
    bus.writeWord(10, 0x00FF);
    cpu->setPc(10);
    ASSERT_EQ("ORI $ff,CCR", subject->disassemble(0b0000000000111100));
}

TEST_F(ORItoCCRTest, ItCorrectlyUpdatesTheCcr) {
    bus.writeWord(10, 0b0000000000010101);
    cpu->setPc(10);
    cpu->setCcrFlags(0);

    subject->execute(0b0000000000111100);

    ASSERT_EQ(CCR_EXTEND | CCR_ZERO | CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ORItoCCRTest, ItReturnsTheCorrectNumberOfClockCycles) {
    ASSERT_EQ(20, subject->execute(0b0000000000111100));
}