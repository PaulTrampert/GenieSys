//
// Created by paul.trampert on 11/30/2020.
//

#include <GenieSys/numberUtils.h>

uint8_t bytesToByte(std::vector<uint8_t> data) {
    return data[0];
}

uint16_t bytesToWord(std::vector<uint8_t> data) {
    return (data[0] << 8) | data[1];
}

uint32_t bytesToLong(std::vector<uint8_t> data) {
    return (data[0] << 24) |
            (data[1] << 16) |
            (data[2] << 8) |
            data [3];
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
