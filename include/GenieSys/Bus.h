//
// Created by paul.trampert on 11/8/2020.
//

#pragma once
#include <array>
#include <vector>
#include <cstdint>
#include <thread>

namespace GenieSys {
#define RAM_SIZE (1024*72)
    class M68kCpu;

    class Bus {
    private:
        std::vector<uint8_t> ram = std::vector<uint8_t>(RAM_SIZE);
        GenieSys::M68kCpu *cpu;

        bool running = false;
        std::thread runThread;

        bool ownsCpu = false;

        virtual void run();

    public:
        Bus();
        Bus(M68kCpu *cpu);
        ~Bus();

        /**
         * Read 1 byte from the given address.
         * @param addr The address to read at.
         * @return The byte at the given address.
         */
        virtual uint8_t read(uint32_t addr);

        /**
         * Read size bytes, starting at the given address.
         * @param addr The address to read from.
         * @param size The number of bytes to read.
         * @return The bytes at that address.
         */
        virtual std::vector<uint8_t> read(uint32_t addr, uint8_t size);

        /**
         * Read 2 bytes as a uint16_t from the given address.
         * @param addr The address to read from.
         * @return The two bytes starting at that address.
         */
        virtual uint16_t readWord(uint32_t addr);

        /**
         * Read 4 bytes as a uint32_t from the given address.
         * @param addr The address to read from
         * @return The four bytes starting at that address.
         */
        virtual uint32_t readLong(uint32_t addr);

        /**
         * Writes the given data starting at the given address.
         * @param addr The address to write to.
         * @param data The data to write.
         */
        virtual void write(uint32_t addr, std::vector<uint8_t> data);

        /**
         * Write a byte at the given address.
         * @param addr The address to writeByte to
         * @param data The byte to writeByte.
         */
        virtual void writeByte(uint32_t addr, uint8_t data);

        /**
         * Write a 16-bit value at the given address.
         * @param addr The address to writeByte to.
         * @param data The data to writeByte.
         */
        virtual void writeWord(uint32_t addr, uint16_t data);

        /**
         * Write a 32-bit value at the given address.
         * @param addr The address to writeByte to.
         * @param data The data to writeByte.
         */
        virtual void writeLong(uint32_t addr, uint32_t data);

        /**
         * Get a reference to the Bus's M68kCpu.
         * @return a reference to the Bus's M68kCpu.
         */
        virtual GenieSys::M68kCpu* getCpu();

        /**
         * Reset all devices on the bus.
         */
        virtual void reset();

        /**
         * Stops the system.
         */
        virtual void stop();

        /**
         * Start the system.
         */
        virtual void start();

        /**
         * Gets whether or not the system is running.
         */
        virtual bool isRunning();
    };
}
