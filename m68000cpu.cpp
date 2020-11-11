//
// Created by paul.trampert on 11/8/2020.
//

#include "m68000cpu.h"
#include "Bus.h"

uint8_t m68000cpu::read(uint32_t addr) {
    return bus->read(addr);
}

void m68000cpu::write(uint32_t addr, uint8_t data) {
    bus->write(addr, data);
}

m68000cpu::m68000cpu() = default;

m68000cpu::~m68000cpu() = default;

void m68000cpu::ConnectBus(Bus *bus) {
    this->bus = bus;
}
