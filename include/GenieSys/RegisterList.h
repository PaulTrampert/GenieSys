//
// Created by paul on 5/19/24.
//
#include <cstdint>
#include <string>
#pragma once

namespace GenieSys {
    class RegisterList {
    private:
        uint16_t regList;

    public:
        RegisterList(uint16_t regList, bool isReversed);

        std::string toString();
    };
}
