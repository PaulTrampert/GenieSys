//
// Created by pault on 12/28/2021.
//

#pragma once
#include <gmock/gmock.h>
#include <GenieSys/M68kCpu.h>

namespace GenieSys {
    class MockCpu : public M68kCpu {
    public:
        MOCK_METHOD(void, ConnectBus, (Bus * bus), (override));
        MOCK_METHOD(uint16_t, getCurrentOpWord, (), (override));
        MOCK_METHOD(uint32_t, getDataRegister, (uint8_t addr), (override));
        MOCK_METHOD(void, setDataRegister, (uint8_t addr, uint8_t value), (override));
        MOCK_METHOD(void, setDataRegister, (uint8_t addr, uint16_t value), (override));
        MOCK_METHOD(void, setDataRegister, (uint8_t addr, uint32_t value), (override));
        MOCK_METHOD(uint32_t, getAddressRegister, (uint8_t addr), (override));
        MOCK_METHOD(void, setAddressRegister, (uint8_t addr, uint32_t value), (override));
        MOCK_METHOD(uint32_t, getPc, (), (override));
        MOCK_METHOD(void, setPc, (uint32_t value), (override));
        MOCK_METHOD(void, setCcrFlags, (uint8_t ccr), (override));
        MOCK_METHOD(uint8_t, getCcrFlags, (), (override));
        MOCK_METHOD(void, tick, (), (override));
        MOCK_METHOD(void, setSR, (uint16_t sr), (override));
        MOCK_METHOD(bool, isSupervisor, (), (override));
        MOCK_METHOD(void, setSupervisor, (bool isSupervisor), (override));
        MOCK_METHOD(uint8_t, trap, (uint8_t vector), (override));
        MOCK_METHOD(AddressingMode *, getAddressingMode, (int modeId), (override));
        MOCK_METHOD(void, stackPush, (uint16_t value), (override));
        MOCK_METHOD(void, stackPush, (uint32_t value), (override));
        MOCK_METHOD(uint16_t, stackPopWord, (), (override));
        MOCK_METHOD(uint32_t, stackPopLong, (), (override));
        MOCK_METHOD(uint8_t, getUspRegister, (), (override));
        MOCK_METHOD(uint32_t, getUserStackPointer, (), (override));
        MOCK_METHOD(void, setUserStackPointer, (uint32_t addr), (override));
        MOCK_METHOD(uint32_t, getStackPointer, (), (override));
        MOCK_METHOD(void, setStackPointer, (uint32_t addr), (override));
        MOCK_METHOD(void, reset, (), (override));
    };
}
