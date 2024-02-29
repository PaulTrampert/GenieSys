//
// Created by paul on 12/7/22.
//
#include "GenieSys/TrapException.h"

GenieSys::TrapException::TrapException(int trapVector) {
    this->trapVector = trapVector;
}

uint32_t GenieSys::TrapException::getTrapVector() const {
    return trapVector;
}

