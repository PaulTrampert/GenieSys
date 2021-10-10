//
// Created by paul.trampert on 11/24/2020.
//

#pragma once

#include <gtest/gtest.h>
#include <GenieSys/Bus.h>


struct BusTest : testing::Test {
    GenieSys::Bus* bus;

    BusTest();
    virtual ~BusTest();
};