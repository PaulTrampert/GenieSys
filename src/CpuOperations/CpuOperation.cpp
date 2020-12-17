//
// Created by paul.trampert on 12/6/2020.
//

#include <GenieSys/CpuOperations/CpuOperation.h>
#include <GenieSys/CpuOperations/Abcd.h>
#include <GenieSys/CpuOperations/Add.h>
#include <algorithm>

CpuOperation::CpuOperation(M68kCpu *cpu, Bus *bus) {
    this->cpu = cpu;
    this->bus = bus;
}

std::vector<CpuOperation *> getOperations(M68kCpu *cpu, Bus *bus) {
    std::vector<CpuOperation*> operations{
            new Abcd(cpu, bus),
            new Add(cpu, bus)
    };

    std::sort(operations.begin(), operations.end(), compare);
    return operations;
}

bool compare(CpuOperation *a, CpuOperation *b) {
    return a->getSpecificity() > b->getSpecificity();
}
