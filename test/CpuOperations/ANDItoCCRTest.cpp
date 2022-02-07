//
// Created by paul.trampert on 12/16/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/ANDItoCCR.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>


struct ANDItoCCRTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    GenieSys::Bus bus;
    GenieSys::ANDItoCCR* subject;

    ANDItoCCRTest() {
        cpu = bus.getCpu();
        subject = new GenieSys::ANDItoCCR(cpu, &bus);
    }

    ~ANDItoCCRTest() override {
        delete subject;
    }
};

TEST_F(ANDItoCCRTest, ItHasTheCorrectSpecificity) {
    ASSERT_EQ(0, subject->getSpecificity());
}

TEST_F(ANDItoCCRTest, ItHasTheCorrectOpcodes) {
    ASSERT_EQ(1, subject->getOpcodes().size());
    ASSERT_EQ(0b0000001000111100, subject->getOpcodes()[0]);
}

TEST_F(ANDItoCCRTest, ItDisassemblesCorrectly) {
    bus.writeWord(10, 0x00FF);
    cpu->setPc(10);
    ASSERT_EQ("ANDI $ff,CCR", subject->disassemble(0b0000000000111100));
}

TEST_F(ANDItoCCRTest, ItCorrectlyUpdatesTheCcr) {
    bus.writeWord(10, 0b0000000000010101);
    cpu->setPc(10);
    cpu->setCcrFlags(0b11111111);

    subject->execute(0b0000000000111100);

    ASSERT_EQ(GenieSys::CCR_EXTEND | GenieSys::CCR_ZERO | GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(ANDItoCCRTest, ItReturnsTheCorrectNumberOfClockCycles) {
    ASSERT_EQ(20, subject->execute(0b0000000000111100));
}