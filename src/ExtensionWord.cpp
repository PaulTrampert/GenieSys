//
// Created by paul.trampert on 11/26/2020.
//

#include "GenieSys/ExtensionWord.h"

ExtensionWord::ExtensionWord() {
    word = 0;
}

ExtensionWord::ExtensionWord(const uint16_t word) {
    this->word = word;
}

ExtensionWord& ExtensionWord::operator=(const uint16_t value) {
    word = value;
    return *this;
}

M68K_REG_TYPE ExtensionWord::getIdxRegType() {
    return static_cast<M68K_REG_TYPE>(word >> 15);
}

void ExtensionWord::setIdxRegType(M68K_REG_TYPE type) {
    word |= (type << 15);
}

uint8_t ExtensionWord::getIdxRegAddr() {
    return static_cast<uint8_t>((word & 0x7000) >> 12);
}

void ExtensionWord::setIdxRegAddr(uint8_t value) {
    word |= ((value & 0x07) << 12);
}

EXT_WORD_IDX_SIZE ExtensionWord::getIdxSize() {
    return static_cast<EXT_WORD_IDX_SIZE>((word & 0x0800) >> 11);
}

void ExtensionWord::setIdxSize(EXT_WORD_IDX_SIZE size) {
    word |= (size << 11);
}

uint8_t ExtensionWord::getScale() {
    return static_cast<uint8_t>((word & 0x0600) >> 9);
}

void ExtensionWord::setScale(uint8_t scale) {
    word |= ((scale & 0x03) << 9);
}

bool ExtensionWord::getBaseRegSuppress() {
    return static_cast<bool>(word & 0x0080);
}

void ExtensionWord::setBaseRegSuppress(bool suppress) {
    word |= suppress << 7;
}

bool ExtensionWord::getIndexSuppress() {
    return static_cast<bool>(word & 0x0040);
}

void ExtensionWord::setIndexSuppress(bool suppress) {
    word |= suppress << 6;
}

EXT_WORD_BD_SIZE ExtensionWord::getBaseDisplacementSize() {
    return static_cast<EXT_WORD_BD_SIZE>((word & 0x0030) >> 4);
}

void ExtensionWord::setBaseDisplacementSize(EXT_WORD_BD_SIZE size) {
    word |= (size << 4);
}

uint8_t ExtensionWord::getIndexIndirectSelection() {
    return static_cast<uint8_t>(word & 0x0007);
}

void ExtensionWord::setIndexIndirectSelection(uint8_t sel) {
    word |= (sel & 0x07);
}

bool ExtensionWord::isBrief() {
    return !(word & 0x0100);
}

void ExtensionWord::setIsBrief(bool isBrief) {
    word |= ((!isBrief) << 8);
}

int8_t ExtensionWord::getDisplacement() {
    return word & 0x00FF;
}

void ExtensionWord::setDisplacement(int8_t displacement) {
    word |= (uint8_t)displacement;
}


