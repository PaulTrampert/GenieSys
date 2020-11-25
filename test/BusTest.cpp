//
// Created by paul.trampert on 11/24/2020.
//

#include "BusTest.h"
#include "GenieSys/Bus.h"

BusTest::BusTest() {
    bus = new Bus();
}

BusTest::~BusTest() {
    delete bus;
}

TEST_F(BusTest, RamIsInitializedToZeros) {
    for (uint32_t i = 0; i < RAM_SIZE; i++) {
        EXPECT_EQ(0, bus->read(i));
    }
}