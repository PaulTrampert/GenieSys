//
// Created by paul.trampert on 11/26/2020.
//
#include "GenieSys/BriefExtensionWord.h"

BriefExtensionWord decodeBriefExtensionWord(uint16_t word) {
    return BriefExtensionWord {
            static_cast<M68K_REG_TYPE>(word >> 15),
            static_cast<uint8_t>((word & 0x7000) >> 12),
            static_cast<EXT_WORD_IDX_SIZE>((word & 0x0800) >> 11),
            static_cast<uint8_t>((word & 0x0600) >> 9),
            static_cast<int8_t>((word & 0x00FF))
    };
}