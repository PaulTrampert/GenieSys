//
// Created by paul.trampert on 12/6/2020.
//

#pragma once
#include "./CpuOperation.h"

/**
 * Add Decimal With Extend (M68KPM.pdf, page 107)
 */
class Abcd : CpuOperation {

public:
    Abcd(M68kCpu* cpu, Bus* bus);
    void execute(uint16_t opWord) override;
};