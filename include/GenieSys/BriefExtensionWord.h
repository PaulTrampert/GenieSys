//
// Created by paul.trampert on 11/26/2020.
//

#pragma once
#include <cstdint>
#include "enums.h"

struct BriefExtensionWord {
    M68K_REG_TYPE idxRegType;
    uint8_t idxRegAddr;
    EXT_WORD_IDX_SIZE idxSize;
    uint8_t scale;
    int8_t displacement;
};

/**
 * Decodes the brief extention word format.
 * @param word The 16-bit brief extension word.
 * @return A struct separating out the fields of the brief extention word.
 */
BriefExtensionWord decodeBriefExtensionWord(uint16_t word);