//
// Created by paul.trampert on 11/26/2020.
//

#include "GenieSys/ExtensionWord.h"

GenieSys::ExtensionWord::ExtensionWord() {
    word = 0;
}

GenieSys::ExtensionWord::ExtensionWord(const uint16_t word) {
    this->word = word;
}

GenieSys::ExtensionWord& GenieSys::ExtensionWord::operator=(const uint16_t value) {
    word = value;
    return *this;
}

GenieSys::M68K_REG_TYPE GenieSys::ExtensionWord::getIdxRegType() {
    return static_cast<M68K_REG_TYPE>(word >> 15);
}

void GenieSys::ExtensionWord::setIdxRegType(M68K_REG_TYPE type) {
    word |= (type << 15);
}

uint8_t GenieSys::ExtensionWord::getIdxRegAddr() {
    return static_cast<uint8_t>((word & 0x7000) >> 12);
}

void GenieSys::ExtensionWord::setIdxRegAddr(uint8_t value) {
    word |= ((value & 0x07) << 12);
}

GenieSys::EXT_WORD_IDX_SIZE GenieSys::ExtensionWord::getIdxSize() {
    return static_cast<EXT_WORD_IDX_SIZE>((word & 0x0800) >> 11);
}

void GenieSys::ExtensionWord::setIdxSize(EXT_WORD_IDX_SIZE size) {
    word |= (size << 11);
}

uint8_t GenieSys::ExtensionWord::getScale() {
    return static_cast<uint8_t>((word & 0x0600) >> 9);
}

void GenieSys::ExtensionWord::setScale(uint8_t scale) {
    word |= ((scale & 0x03) << 9);
}

bool GenieSys::ExtensionWord::getBaseRegSuppress() {
    return static_cast<bool>(word & 0x0080);
}

void GenieSys::ExtensionWord::setBaseRegSuppress(bool suppress) {
    word |= suppress << 7;
}

bool GenieSys::ExtensionWord::getIndexSuppress() {
    return static_cast<bool>(word & 0x0040);
}

void GenieSys::ExtensionWord::setIndexSuppress(bool suppress) {
    word |= suppress << 6;
}

GenieSys::EXT_WORD_BD_SIZE GenieSys::ExtensionWord::getBaseDisplacementSize() {
    return static_cast<EXT_WORD_BD_SIZE>((word & 0x0030) >> 4);
}

void GenieSys::ExtensionWord::setBaseDisplacementSize(EXT_WORD_BD_SIZE size) {
    word |= (size << 4);
}

uint8_t GenieSys::ExtensionWord::getIndexIndirectSelection() {
    return static_cast<uint8_t>(word & 0x0007);
}

void GenieSys::ExtensionWord::setIndexIndirectSelection(uint8_t sel) {
    word |= (sel & 0x07);
}

bool GenieSys::ExtensionWord::isBrief() {
    return !(word & 0x0100);
}

void GenieSys::ExtensionWord::setIsBrief(bool isBrief) {
    word |= ((!isBrief) << 8);
}

int8_t GenieSys::ExtensionWord::getDisplacement() {
    return word & 0x00FF;
}

void GenieSys::ExtensionWord::setDisplacement(int8_t displacement) {
    word |= (uint8_t)displacement;
}


