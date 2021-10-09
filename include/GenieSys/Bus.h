//
// Created by paul.trampert on 11/8/2020.
//

#pragma once
#include <array>
#include <vector>
#include <cstdint>
#include "M68kCpu.h"

namespace GenieSys {
#define RAM_SIZE 1024*72

    class Bus {
    private:
        std::vector<uint8_t> ram = std::vector<uint8_t>(RAM_SIZE);
        GenieSys::M68kCpu cpu;

    public:
        Bus();
        ~Bus();

        /**
         * Read 1 byte from the given address.
         * @param addr The address to read at.
         * @return The byte at the given address.
         */
        uint8_t read(uint32_t addr);

        /**
         * Read size bytes, starting at the given address.
         * @param addr The address to read from.
         * @param size The number of bytes to read.
         * @return The bytes at that address.
         */
        std::vector<uint8_t> read(uint32_t addr, uint8_t size);

        /**
         * Read 2 bytes as a uint16_t from the given address.
         * @param addr The address to read from.
         * @return The two bytes starting at that address.
         */
        uint16_t readWord(uint32_t addr);

        /**
         * Read 4 bytes as a uint32_t from the given address.
         * @param addr The address to read from
         * @return The four bytes starting at that address.
         */
        uint32_t readLong(uint32_t addr);

        /**
         * Writes the given data starting at the given address.
         * @param addr The address to write to.
         * @param data The data to write.
         */
        void write(uint32_t addr, std::vector<uint8_t> data);

        /**
         * Write a byte at the given address.
         * @param addr The address to writeByte to
         * @param data The byte to writeByte.
         */
        void writeByte(uint32_t addr, uint8_t data);

        /**
         * Write a 16-bit value at the given address.
         * @param addr The address to writeByte to.
         * @param data The data to writeByte.
         */
        void writeWord(uint32_t addr, uint16_t data);

        /**
         * Write a 32-bit value at the given address.
         * @param addr The address to writeByte to.
         * @param data The data to writeByte.
         */
        void writeLong(uint32_t addr, uint32_t data);

        /**
         * Get a reference to the Bus's M68kCpu.
         * @return a reference to the Bus's M68kCpu.
         */
        GenieSys::M68kCpu* getCpu();
    };
}


