//
// Created by paul.trampert on 11/8/2020.
//

#pragma once
#include <array>
#include <cstdint>
#include "m68000cpu.h"

#define RAM_SIZE 1024*72

class Bus {
private:
    std::array<uint8_t, RAM_SIZE> ram;
    m68000cpu cpu;

public:
    Bus();
    ~Bus();
    uint8_t read(uint32_t addr);
    void write(uint32_t addr, uint8_t data);
};


