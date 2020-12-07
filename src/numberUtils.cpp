//
// Created by paul.trampert on 11/30/2020.
//

#include <GenieSys/numberUtils.h>

uint8_t bytesToByte(std::vector<uint8_t> data) {
    return data[data.size() - 1];
}

uint16_t bytesToWord(std::vector<uint8_t> data) {
    return (data[data.size() - 2] << 8) | data[data.size() - 1];
}

uint32_t bytesToLong(std::vector<uint8_t> data) {
    return (data[data.size() - 4] << 24) |
            (data[data.size() - 3] << 16) |
            (data[data.size() - 2] << 8) |
            data [data.size() - 1];
}

std::vector<uint8_t> getBytes(uint8_t byte) {
    return std::vector<uint8_t>{byte};
}

std::vector<uint8_t> getBytes(uint16_t byte) {
    return std::vector<uint8_t>{
            static_cast<uint8_t>((byte & 0xFF00) >> 8),
            static_cast<uint8_t>(byte & 0x00FF)
    };
}

std::vector<uint8_t> getBytes(uint32_t byte) {
    return std::vector<uint8_t> {
        static_cast<uint8_t>((byte & 0xFF000000) >> 24),
        static_cast<uint8_t>((byte & 0x00FF0000) >> 16),
        static_cast<uint8_t>((byte & 0x0000FF00) >> 8),
        static_cast<uint8_t>((byte & 0x000000FF)),
    };
}
