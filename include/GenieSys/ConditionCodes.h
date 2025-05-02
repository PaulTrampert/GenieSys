//
// Created by paul on 5/2/25.
//

#pragma once

#define CC_T  0b0000
#define CC_F  0b0001
#define CC_CC 0b0100
#define CC_CS 0b0101
#define CC_EQ 0b0111
#define CC_GE 0b1100
#define CC_GT 0b1110
#define CC_HI 0b0010
#define CC_LE 0b1111
#define CC_LS 0b0011
#define CC_LT 0b1101
#define CC_MI 0b1011
#define CC_NE 0b0110
#define CC_PL 0b1010
#define CC_VC 0b1000
#define CC_VS 0b1001

namespace GenieSys {

    std::string getConditionCodeName(uint8_t conditionCode);
}