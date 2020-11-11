//
// Created by paul.trampert on 11/8/2020.
//

#include "Bus.h"


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
