//
// Created by pault on 12/29/2021.
//

#include <gtest/gtest.h>
#include "./WithM68kCpu.h"

struct SetDataRegisterTestParams {
    uint8_t byteValue;
    uint16_t wordValue;
    uint32_t longValue;

    uint32_t initValue;

    uint8_t address;

    uint32_t expectedAfterSetByte;
    uint32_t expectedAfterSetWord;
    uint32_t expectedAfterSetLong;
};

struct SetDataRegisterTests :
        public WithM68KCpu,
        public testing::TestWithParam<SetDataRegisterTestParams> {

};

TEST_P(SetDataRegisterTests, SetByte) {
    auto params = GetParam();
    subject->setDataRegister(params.address, params.initValue);

    subject->setDataRegister(params.address, params.byteValue);

    ASSERT_EQ(params.expectedAfterSetByte, subject->getDataRegister(params.address));
}

TEST_P(SetDataRegisterTests, SetWord) {
    auto params = GetParam();
    subject->setDataRegister(params.address, params.initValue);

    subject->setDataRegister(params.address, params.wordValue);

    ASSERT_EQ(params.expectedAfterSetWord, subject->getDataRegister(params.address));
}

TEST_P(SetDataRegisterTests, SetLong) {
    auto params = GetParam();
    subject->setDataRegister(params.address, params.initValue);

    subject->setDataRegister(params.address, params.longValue);

    ASSERT_EQ(params.expectedAfterSetLong, subject->getDataRegister(params.address));
}

INSTANTIATE_TEST_SUITE_P(M68kCpuTests, SetDataRegisterTests,
    testing::Values(
        SetDataRegisterTestParams {
            .byteValue = 0x00,
            .wordValue = 0x0000,
            .longValue = 0x00000000,
            .initValue = 0xFFFFFFFF,
            .address = 0,
            .expectedAfterSetByte = 0xFFFFFF00,
            .expectedAfterSetWord = 0xFFFF0000,
            .expectedAfterSetLong = 0x00000000
        },
        SetDataRegisterTestParams {
            .byteValue = 0x00,
            .wordValue = 0x0000,
            .longValue = 0x00000000,
            .initValue = 0xFFFFFFFF,
            .address = 1,
            .expectedAfterSetByte = 0xFFFFFF00,
            .expectedAfterSetWord = 0xFFFF0000,
            .expectedAfterSetLong = 0x00000000
        },
        SetDataRegisterTestParams {
            .byteValue = 0x00,
            .wordValue = 0x0000,
            .longValue = 0x00000000,
            .initValue = 0xFFFFFFFF,
            .address = 2,
            .expectedAfterSetByte = 0xFFFFFF00,
            .expectedAfterSetWord = 0xFFFF0000,
            .expectedAfterSetLong = 0x00000000
        },
        SetDataRegisterTestParams {
            .byteValue = 0x00,
            .wordValue = 0x0000,
            .longValue = 0x00000000,
            .initValue = 0xFFFFFFFF,
            .address = 3,
            .expectedAfterSetByte = 0xFFFFFF00,
            .expectedAfterSetWord = 0xFFFF0000,
            .expectedAfterSetLong = 0x00000000
        },
        SetDataRegisterTestParams {
            .byteValue = 0x00,
            .wordValue = 0x0000,
            .longValue = 0x00000000,
            .initValue = 0xFFFFFFFF,
            .address = 4,
            .expectedAfterSetByte = 0xFFFFFF00,
            .expectedAfterSetWord = 0xFFFF0000,
            .expectedAfterSetLong = 0x00000000
        },
        SetDataRegisterTestParams {
            .byteValue = 0x00,
            .wordValue = 0x0000,
            .longValue = 0x00000000,
            .initValue = 0xFFFFFFFF,
            .address = 5,
            .expectedAfterSetByte = 0xFFFFFF00,
            .expectedAfterSetWord = 0xFFFF0000,
            .expectedAfterSetLong = 0x00000000
        },
        SetDataRegisterTestParams {
            .byteValue = 0x00,
            .wordValue = 0x0000,
            .longValue = 0x00000000,
            .initValue = 0xFFFFFFFF,
            .address = 6,
            .expectedAfterSetByte = 0xFFFFFF00,
            .expectedAfterSetWord = 0xFFFF0000,
            .expectedAfterSetLong = 0x00000000
        },
        SetDataRegisterTestParams {
            .byteValue = 0x00,
            .wordValue = 0x0000,
            .longValue = 0x00000000,
            .initValue = 0xFFFFFFFF,
            .address = 7,
            .expectedAfterSetByte = 0xFFFFFF00,
            .expectedAfterSetWord = 0xFFFF0000,
            .expectedAfterSetLong = 0x00000000
        },
        SetDataRegisterTestParams {
                .byteValue = 0xFF,
                .wordValue = 0xFFFF,
                .longValue = 0xFFFFFFFF,
                .initValue = 0x00000000,
                .address = 0,
                .expectedAfterSetByte = 0x000000FF,
                .expectedAfterSetWord = 0x0000FFFF,
                .expectedAfterSetLong = 0xFFFFFFFF
        },
        SetDataRegisterTestParams {
                .byteValue = 0xFF,
                .wordValue = 0xFFFF,
                .longValue = 0xFFFFFFFF,
                .initValue = 0x00000000,
                .address = 1,
                .expectedAfterSetByte = 0x000000FF,
                .expectedAfterSetWord = 0x0000FFFF,
                .expectedAfterSetLong = 0xFFFFFFFF
        },
        SetDataRegisterTestParams {
                .byteValue = 0xFF,
                .wordValue = 0xFFFF,
                .longValue = 0xFFFFFFFF,
                .initValue = 0x00000000,
                .address = 2,
                .expectedAfterSetByte = 0x000000FF,
                .expectedAfterSetWord = 0x0000FFFF,
                .expectedAfterSetLong = 0xFFFFFFFF
        },
        SetDataRegisterTestParams {
                .byteValue = 0xFF,
                .wordValue = 0xFFFF,
                .longValue = 0xFFFFFFFF,
                .initValue = 0x00000000,
                .address = 3,
                .expectedAfterSetByte = 0x000000FF,
                .expectedAfterSetWord = 0x0000FFFF,
                .expectedAfterSetLong = 0xFFFFFFFF
        },
        SetDataRegisterTestParams {
                .byteValue = 0xFF,
                .wordValue = 0xFFFF,
                .longValue = 0xFFFFFFFF,
                .initValue = 0x00000000,
                .address = 4,
                .expectedAfterSetByte = 0x000000FF,
                .expectedAfterSetWord = 0x0000FFFF,
                .expectedAfterSetLong = 0xFFFFFFFF
        },
        SetDataRegisterTestParams {
                .byteValue = 0xFF,
                .wordValue = 0xFFFF,
                .longValue = 0xFFFFFFFF,
                .initValue = 0x00000000,
                .address = 5,
                .expectedAfterSetByte = 0x000000FF,
                .expectedAfterSetWord = 0x0000FFFF,
                .expectedAfterSetLong = 0xFFFFFFFF
        },
        SetDataRegisterTestParams {
                .byteValue = 0xFF,
                .wordValue = 0xFFFF,
                .longValue = 0xFFFFFFFF,
                .initValue = 0x00000000,
                .address = 6,
                .expectedAfterSetByte = 0x000000FF,
                .expectedAfterSetWord = 0x0000FFFF,
                .expectedAfterSetLong = 0xFFFFFFFF
        },
        SetDataRegisterTestParams {
                .byteValue = 0xFF,
                .wordValue = 0xFFFF,
                .longValue = 0xFFFFFFFF,
                .initValue = 0x00000000,
                .address = 7,
                .expectedAfterSetByte = 0x000000FF,
                .expectedAfterSetWord = 0x0000FFFF,
                .expectedAfterSetLong = 0xFFFFFFFF
        }
    )
);