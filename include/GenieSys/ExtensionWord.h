//
// Created by paul.trampert on 11/26/2020.
//

#pragma once
#include <cstdint>
#include "enums.h"

struct ExtensionWord {
    M68K_REG_TYPE idxRegType;
    uint8_t idxRegAddr;
    EXT_WORD_IDX_SIZE idxSize;
    uint8_t scale;
    bool baseRegSuppress;
    bool indexSuppress;
    EXT_WORD_BD_SIZE baseDisplacementSize;
    uint8_t indexIndirectSelection;
};

/**
 * Decodes the full extension word format.
 * @param word The 16-bit extension word.
 * @return A struct separating out the fields of the full extension word.
 */
ExtensionWord decodeExtensionWord(uint16_t word);