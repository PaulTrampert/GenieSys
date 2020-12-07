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
    CpuOperation(M68kCpu* cpu, Bus* bus);
    virtual void execute(uint16_t opWord) = 0;
};