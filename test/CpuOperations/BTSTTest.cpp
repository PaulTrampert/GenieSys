//
// Created by paul.trampert on 4/4/2021.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/BTST.h>

struct BTSTTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    BTST* subject;

    BTSTTest() {
        cpu = bus.getCpu();
        subject = new BTST(cpu, &bus);
    }

    ~BTSTTest() override {
        delete subject;
    }
};


TEST_F(BTSTTest, SpecificityIsTen) {
    ASSERT_EQ(10, subject->getSpecificity());
}