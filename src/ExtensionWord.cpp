//
// Created by paul.trampert on 11/26/2020.
//

#include "GenieSys/ExtensionWord.h"

ExtensionWord decodeExtensionWord(uint16_t word) {
    return ExtensionWord {
            static_cast<M68K_REG_TYPE>(word >> 15),
            static_cast<uint8_t>((word & 0x7000) >> 12),
            static_cast<EXT_WORD_IDX_SIZE>((word & 0x0800) >> 11),
            static_cast<uint8_t>((word & 0x0600) >> 9),
            static_cast<bool>(word & 0x0080),
            static_cast<bool>(word & 0x0040),
            static_cast<EXT_WORD_BD_SIZE>((word & 0x0030) >> 4),
            static_cast<uint8_t>(word & 0x0007)
    };
}