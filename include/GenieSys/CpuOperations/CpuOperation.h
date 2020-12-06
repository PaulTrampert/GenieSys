//
// Created by paul.trampert on 12/3/2020.
//

#pragma once
#include "../M68kCpu.h"
#include "../Bus.h"

class CpuOperation {
protected:
    Bus* bus;
    M68kCpu* cpu;

public:
    virtual void execute() = 0;
};