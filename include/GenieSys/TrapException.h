//
// Created by paul on 12/7/22.
//

#pragma once

#include <exception>
#include <string>
#include <cstdint>

namespace GenieSys {
class TrapException : public std::exception {
private:
    int trapVector;
public:
    explicit TrapException(int trapVector);
    [[nodiscard]] uint32_t getTrapVector() const;
};
}