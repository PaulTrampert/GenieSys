//
// Created by pault on 12/28/2021.
//

#pragma once
#include <gmock/gmock.h>
#include <GenieSys/Bus.h>

namespace GenieSys {
    class MockBus : public Bus {
    public:
        MockBus() : Bus() {

        }

        explicit MockBus(M68kCpu *cpu) : Bus(cpu){

        }

        MOCK_METHOD(uint8_t, read, (uint32_t addr), (override));
        MOCK_METHOD(std::vector<uint8_t>, read, (uint32_t addr, uint8_t size), (override));
        MOCK_METHOD(uint16_t, readWord, (uint32_t addr), (override));
        MOCK_METHOD(uint32_t, readLong, (uint32_t addr), (override));
        MOCK_METHOD(void, write, (uint32_t addr, std::vector<uint8_t> data), (override));
        MOCK_METHOD(void, writeByte, (uint32_t addr, uint8_t data), (override));
        MOCK_METHOD(void, writeWord, (uint32_t addr, uint16_t data), (override));
        MOCK_METHOD(void, writeLong, (uint32_t addr, uint32_t data), (override));
        MOCK_METHOD(M68kCpu*, getCpu, (), (override));
        MOCK_METHOD(void, reset, (), (override));
        MOCK_METHOD(void, stop, (), (override));
        MOCK_METHOD(void, start, (), (override));
        MOCK_METHOD(bool, isRunning, (), (override));
    };
}
