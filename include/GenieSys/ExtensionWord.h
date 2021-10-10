//
// Created by paul.trampert on 11/26/2020.
//

#pragma once
#include <cstdint>
#include "enums.h"

namespace GenieSys {
    class ExtensionWord {
    private:
        uint16_t word;

    public:
        ExtensionWord();
        explicit ExtensionWord(uint16_t word);
        ~ExtensionWord() = default;

        GenieSys::M68K_REG_TYPE getIdxRegType();
        void setIdxRegType(GenieSys::M68K_REG_TYPE type);

        uint8_t getIdxRegAddr();
        void setIdxRegAddr(uint8_t value);

        GenieSys::EXT_WORD_IDX_SIZE getIdxSize();
        void setIdxSize(GenieSys::EXT_WORD_IDX_SIZE size);

        uint8_t getScale();
        void setScale(uint8_t scale);

        bool getBaseRegSuppress();
        void setBaseRegSuppress(bool suppress);

        bool getIndexSuppress();
        void setIndexSuppress(bool suppress);

        GenieSys::EXT_WORD_BD_SIZE getBaseDisplacementSize();
        void setBaseDisplacementSize(GenieSys::EXT_WORD_BD_SIZE size);

        uint8_t getIndexIndirectSelection();
        void setIndexIndirectSelection(uint8_t sel);

        bool isBrief();
        void setIsBrief(bool isBrief);

        int8_t getDisplacement();
        void setDisplacement(int8_t displacement);

        GenieSys::ExtensionWord& operator = (uint16_t value);
        explicit operator uint16_t() const { return word; };
    };
}

