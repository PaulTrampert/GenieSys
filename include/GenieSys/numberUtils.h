//
// Created by paul.trampert on 11/30/2020.
//

#pragma once
#include <vector>
#include <cstdint>
#include <string>

uint8_t bytesToByte(std::vector<uint8_t> data);

uint16_t bytesToWord(std::vector<uint8_t> data);

uint32_t bytesToLong(std::vector<uint8_t> data);

std::vector<uint8_t> getBytes(uint8_t byte);

std::vector<uint8_t> getBytes(uint16_t byte);

std::vector<uint8_t> getBytes(uint32_t byte);

std::string toHex(std::vector<uint8_t> data);