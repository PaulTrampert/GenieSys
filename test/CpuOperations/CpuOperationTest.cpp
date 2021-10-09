//
// Created by paul.trampert on 12/16/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/CpuOperations/CpuOperation.h>



TEST(CpuOperationTest, getOperationsReturnsOperationsInTheCorrectOrder) {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu = bus.getCpu();
    CpuOperation* previous = nullptr;
    auto operations = getOperations(cpu, &bus);
    for(auto & op : operations) {
        if (previous != nullptr) {
            ASSERT_TRUE(previous->getSpecificity() >= op->getSpecificity());
        }
        previous = op.get();
    }
}