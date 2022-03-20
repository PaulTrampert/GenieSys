//
// Created by paul on 3/17/22.
//

#pragma once
#include <gmock/gmock.h>
#include <GenieSys/AddressingModes/AddressingMode.h>

class MockAddressingResult : public GenieSys::AddressingResult {
public:
    MockAddressingResult():AddressingResult(nullptr, nullptr, 0, std::vector<uint8_t>(), 0, 0) {}
    MOCK_METHOD(uint8_t, getCycles, (), (override));
    MOCK_METHOD(uint8_t, getMoveCycleKey, (), (override));
    MOCK_METHOD(std::vector<uint8_t>, getData, (), (override));
    MOCK_METHOD(uint8_t, getDataAsByte, (), (override));
    MOCK_METHOD(uint16_t, getDataAsWord, (), (override));
    MOCK_METHOD(uint32_t, getDataAsLong, (), (override));
    MOCK_METHOD(uint32_t, getAddress, (), (override));
    MOCK_METHOD(void, write, (std::vector<uint8_t> data), (override));
    MOCK_METHOD(void, write, (uint8_t data), (override));
    MOCK_METHOD(void, write, (uint16_t data), (override));
    MOCK_METHOD(void, write, (uint32_t data), (override));
};

class MockAddressingMode : public GenieSys::AddressingMode {
public:
    MockAddressingMode() : AddressingMode(nullptr, nullptr) {}

    std::unique_ptr<GenieSys::AddressingResult> getData(uint8_t regAddr, uint8_t size) override {
        return std::unique_ptr<GenieSys::AddressingResult>(getDataProxy(regAddr, size));
    }

    MOCK_METHOD(uint32_t, getAddress, (uint8_t regAddr), (override));
    MOCK_METHOD(GenieSys::AddressingResult *, getDataProxy, (uint8_t regAddr, uint8_t size), ());
    MOCK_METHOD(uint8_t, getModeId, (), (override));
    MOCK_METHOD(void, setBus, (GenieSys::Bus *b), (override));
    MOCK_METHOD(std::string, disassemble, (uint8_t regAddr, uint8_t size), (override));
    MOCK_METHOD(uint8_t, getMoveCycleKey, (), (override));
};