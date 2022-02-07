//
// Created by paul.trampert on 11/24/2020.
//

#include "BusTest.h"
#include "GenieSys/Bus.h"
#include <thread>
#include <chrono>


BusTest::BusTest() {
    cpu = new GenieSys::MockCpu();
    EXPECT_CALL(*cpu, ConnectBus(testing::NotNull()));
    bus = new GenieSys::Bus(cpu);
}

BusTest::~BusTest() {
    delete bus;
    delete cpu;
}

TEST_F(BusTest, RamIsInitializedToZeros) {
    for (uint32_t i = 0; i < RAM_SIZE; i++) {
        EXPECT_EQ(0, bus->read(i));
    }
}

TEST_F(BusTest, WritingBytesToRamWorks) {
    bus->writeByte(5, 42);
    EXPECT_EQ(42, bus->read(5));
}

TEST_F(BusTest, WritingSmallWordsToRamWorks) {
    bus->writeWord(5,42);
    EXPECT_EQ(42, bus->readWord(5));
    EXPECT_EQ(0, bus->read(5));
    EXPECT_EQ(42, bus->read(6));
}

TEST_F(BusTest, WritingBiggerWordsToRamWorks) {
    bus->writeWord(5, 0xCD42);
    EXPECT_EQ(0xCD42, bus->readWord(5));
    EXPECT_EQ(0xCD, bus->read(5));
    EXPECT_EQ(0x42, bus->read(6));
}

TEST_F(BusTest, WritingLongsToRamWorks) {
    bus->writeLong(4, 0xABCD1234);
    EXPECT_EQ(0xABCD1234, bus->readLong(4));
    EXPECT_EQ(0xAB, bus->read(4));
    EXPECT_EQ(0xCD, bus->read(5));
    EXPECT_EQ(0x12, bus->read(6));
    EXPECT_EQ(0x34, bus->read(7));
}

TEST_F(BusTest, StartAndStop) {
    EXPECT_CALL(*cpu, tick()).Times(testing::AtLeast(1));
    bus->start();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_TRUE(bus->isRunning());
    bus->stop();
    EXPECT_FALSE(bus->isRunning());
}