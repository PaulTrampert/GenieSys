//
// Created by paul.trampert on 11/8/2020.
//

#include <vector>
#include <thread>
#include "GenieSys/Bus.h"



GenieSys::Bus::Bus() {
    for (uint32_t i = 0; i < RAM_SIZE; i++) {
        ram[i] = 0x00;
    }
    cpu.ConnectBus(this);
}

GenieSys::Bus::~Bus() {

}

uint8_t GenieSys::Bus::read(uint32_t addr) {
    if (addr < RAM_SIZE)
        return ram[addr];
    return 0;
}

void GenieSys::Bus::writeByte(uint32_t addr, uint8_t data) {
    if (addr < RAM_SIZE) {
        ram[addr] = data;
    }
}

uint16_t GenieSys::Bus::readWord(uint32_t addr) {
    return (read(addr) << 8) | read(addr + 1);
}

uint32_t GenieSys::Bus::readLong(uint32_t addr) {
    return (read(addr) << 24) | (read(addr + 1) << 16) | (read(addr + 2) << 8) | read(addr + 3);
}

void GenieSys::Bus::writeWord(uint32_t addr, uint16_t data) {
    writeByte(addr, (data & 0xFF00) >> 8);
    writeByte(addr + 1, data & 0x00FF);
}

void GenieSys::Bus::writeLong(uint32_t addr, uint32_t data) {
    writeByte(addr, (data & 0xFF000000) >> 24);
    writeByte(addr + 1, (data & 0x00FF0000) >> 16);
    writeByte(addr + 2, (data & 0x0000FF00) >> 8);
    writeByte(addr + 3, (data & 0x000000FF));
}

GenieSys::M68kCpu *GenieSys::Bus::getCpu() {
    return &cpu;
}

std::vector<uint8_t> GenieSys::Bus::read(uint32_t addr, uint8_t size) {
    std::vector<uint8_t> result(size);
    for (int i = 0; i < size; i++) {
        result[i] = read(addr + i);
    }
    return result;
}

void GenieSys::Bus::write(uint32_t addr, std::vector<uint8_t> data) {
    for(int i = 0; i < data.size(); i++) {
        writeByte(addr + i, data[i]);
    }
}

void GenieSys::Bus::reset() {
    cpu.reset();
}

void GenieSys::Bus::stop() {
    running = false;
    runThread.join();
}

void GenieSys::Bus::start() {
    running = true;
    runThread = std::thread(&Bus::run, this);
}

void GenieSys::Bus::run() {
    while (running) {
        cpu.tick();
    }
}

bool GenieSys::Bus::isRunning() {
    return running;
}
