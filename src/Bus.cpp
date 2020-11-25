//
// Created by paul.trampert on 11/8/2020.
//

#include "GenieSys/Bus.h"


Bus::Bus() {
    for (uint32_t i = 0; i < RAM_SIZE; i++) {
        ram[i] = 0x00;
    }
    cpu.ConnectBus(this);
}

Bus::~Bus() {

}

uint8_t Bus::read(uint32_t addr) {
    if (addr < RAM_SIZE)
        return ram[addr];
    return 0;
}

void Bus::write(uint32_t addr, uint8_t data) {
    if (addr < RAM_SIZE) {
        ram[addr] = data;
    }
}

uint16_t Bus::readWord(uint32_t addr) {
    if (addr < RAM_SIZE - 1) {
        return (ram[addr] << 8) | ram[addr + 1];
    }
    return 0;
}

uint32_t Bus::readLong(uint32_t addr) {
    if (addr < RAM_SIZE - 3) {
        return (ram[addr] << 24) | (ram[addr + 1] << 16) | (ram[addr + 2] << 8) | ram[addr + 3];
    }
    return 0;
}
