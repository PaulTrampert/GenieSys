//
// Created by paul on 5/2/25.
//
#include <string>
#include <cstdint>
#include "GenieSys/ConditionCodes.h"

const std::string CC_NAMES[] = {
        "T",
        "F",
        "HI",
        "LS",
        "CC",
        "CS",
        "NE",
        "EQ",
        "VC",
        "VS",
        "PL",
        "MI",
        "GE",
        "LT",
        "GT",
        "LE",
};

std::string GenieSys::getConditionCodeName(uint8_t conditionCode) {
    if (conditionCode > 15) {
        return "Unknown";
    }
    return CC_NAMES[conditionCode];
}