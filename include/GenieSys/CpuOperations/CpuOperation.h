//
// Created by paul.trampert on 12/3/2020.
//

#pragma once
#include <vector>
#include <string>
#include "../M68kCpu.h"
#include "../Bus.h"


class CpuOperation {
protected:
    Bus* bus;
    GenieSys::M68kCpu* cpu;

public:
    CpuOperation(GenieSys::M68kCpu* cpu, Bus* bus);
    virtual ~CpuOperation() = default;
    virtual uint8_t execute(uint16_t opWord) = 0;
    virtual std::vector<uint16_t> getOpcodes() = 0;
    virtual uint8_t getSpecificity() = 0;
    virtual std::string disassemble(uint16_t opWord) = 0;
};

std::vector<std::shared_ptr<CpuOperation>> getOperations(GenieSys::M68kCpu* cpu, Bus* bus);

bool compare(const std::shared_ptr<CpuOperation> &a, const std::shared_ptr<CpuOperation> &b);