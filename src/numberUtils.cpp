//
// Created by paul.trampert on 11/30/2020.
//

#include <GenieSys/numberUtils.h>
#include <iomanip>
#include <sstream>

uint8_t GenieSys::bytesToByte(std::vector<uint8_t> data) {
    return data[data.size() - 1];
}

uint16_t GenieSys::bytesToWord(std::vector<uint8_t> data) {
    return (data[data.size() - 2] << 8) | data[data.size() - 1];
}

uint32_t GenieSys::bytesToLong(std::vector<uint8_t> data) {
    return (data[data.size() - 4] << 24) |
           (data[data.size() - 3] << 16) |
           (data[data.size() - 2] << 8) |
           data [data.size() - 1];
}

std::vector<uint8_t> GenieSys::getBytes(uint8_t byte) {
    return std::vector<uint8_t>{byte};
}

std::vector<uint8_t> GenieSys::getBytes(uint16_t byte) {
    return std::vector<uint8_t>{
            static_cast<uint8_t>((byte & 0xFF00) >> 8),
            static_cast<uint8_t>(byte & 0x00FF)
    };
}

std::vector<uint8_t> GenieSys::getBytes(uint32_t byte) {
    return std::vector<uint8_t> {
            static_cast<uint8_t>((byte & 0xFF000000) >> 24),
            static_cast<uint8_t>((byte & 0x00FF0000) >> 16),
            static_cast<uint8_t>((byte & 0x0000FF00) >> 8),
            static_cast<uint8_t>((byte & 0x000000FF)),
    };
}

std::string GenieSys::toHex(const std::vector<uint8_t>& data) {
    std::stringstream stream;
    stream << std::setfill('0') << std::hex;
    for (auto & d : data) {
        stream << std::setw(2) << (int)d;
    }
    return stream.str();
}

uint16_t GenieSys::bitwiseReverse(uint16_t word) {
    uint16_t newWord = 0;
    for (int i = 0; i < 16; i++) {
        uint16_t nextBit = (word >> (15 - i)) & 1;
        newWord |= nextBit << i;
    }
    return newWord;
}