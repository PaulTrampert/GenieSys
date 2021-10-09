//
// Created by paul.trampert on 12/16/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/EORItoCCR.h>


struct EORItoCCRTest : testing::Test {
    GenieSys::M68kCpu* cpu;
    Bus bus;
    EORItoCCR* subject;

    EORItoCCRTest() {
        cpu = bus.getCpu();
        subject = new EORItoCCR(cpu, &bus);
    }

    ~EORItoCCRTest() override {
        delete subject;
    }
};

TEST_F(EORItoCCRTest, ItHasTheCorrectSpecificity) {
    ASSERT_EQ(0, subject->getSpecificity());
}

TEST_F(EORItoCCRTest, ItHasTheCorrectOpcodes) {
    ASSERT_EQ(1, subject->getOpcodes().size());
    ASSERT_EQ(0b0000101000111100, subject->getOpcodes()[0]);
}

TEST_F(EORItoCCRTest, ItDisassemblesCorrectly) {
    bus.writeWord(10, 0x00FF);
    cpu->setPc(10);
    ASSERT_EQ("EORI $ff,CCR", subject->disassemble(0b0000101000111100));
}

TEST_F(EORItoCCRTest, ItCorrectlyUpdatesTheCcr) {
    bus.writeWord(10, 0b0000000000010101);
    cpu->setPc(10);
    cpu->setCcrFlags(0b11111111);

    subject->execute(0b0000101000111100);

    ASSERT_EQ(0b11101010, cpu->getCcrFlags());
}

TEST_F(EORItoCCRTest, ItReturnsTheCorrectNumberOfClockCycles) {
    ASSERT_EQ(20, subject->execute(0b0000101000111100));
}