//
// Created by paul on 5/19/24.
//
#include <GenieSys/RegisterList.h>
#include <GenieSys/numberUtils.h>
#include <sstream>

GenieSys::RegisterList::RegisterList(uint16_t regList, bool isReversed) {
    this->regList = isReversed ? bitwiseReverse(regList) : regList;
}

std::string registerListByteToString(uint8_t regListByte, char regPrefix) {
    std::stringstream ss;
    bool firstEntry = true;
    for (int i = 0; i < 8; i++) {
        bool bit = (regListByte & (1 << i)) != 0;
        int contiguousBits = 0;
        int firstBit = i;
        while (bit && i < 8) {
            contiguousBits++;
            i++;
            bit = (regListByte & (1 << i)) != 0;
        }
        if (contiguousBits > 0) {
            if (!firstEntry) {
                ss << "/";
            }
            if (contiguousBits == 1) {
                ss << regPrefix << firstBit;
            } else {
                ss << regPrefix << firstBit << (contiguousBits == 2 ? "/" : "-") << regPrefix << (i - 1);
            }
            firstEntry = false;
        }
    }
    return ss.str();
}

std::string GenieSys::RegisterList::toString() {
    std::stringstream ss;
    ss << registerListByteToString(regList & 0xFF, 'D');
    if ((regList & 0xFF00) != 0) {
        ss << "/" << registerListByteToString((regList & 0xFF00) >> 8, 'A');
    }
    return ss.str();
}