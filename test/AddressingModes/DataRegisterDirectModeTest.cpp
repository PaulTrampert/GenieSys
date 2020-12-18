//
// Created by paul.trampert on 12/3/2020.
//

//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>

struct DataRegisterDirectModeTest : testing::Test {
    Bus bus;
    M68kCpu* cpu;
    DataRegisterDirectMode* subject;

    DataRegisterDirectModeTest() {
        cpu = bus.getCpu();
        cpu->setDataRegister(3, (uint32_t)9999);
        subject = new DataRegisterDirectMode(cpu, &bus);
    }

    ~DataRegisterDirectModeTest() override {
        delete subject;
    }
};

TEST_F(DataRegisterDirectModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(3, subject->getAddress(3));
}

TEST_F(DataRegisterDirectModeTest, ItGetsTheCorrectData) {
    std::vector<uint8_t> result = subject->getData(3, 4)->getData();
    uint32_t finalResult = 0;
    for (int i = 0; i < 4; i++) {
        finalResult |= result[i] << (8 * (3 - i));
    }
    EXPECT_EQ(9999, finalResult);
}

TEST_F(DataRegisterDirectModeTest, TestGetModeId) {
    EXPECT_EQ(0b000u, subject->getModeId());
}

TEST_F(DataRegisterDirectModeTest, TestDisassemble) {
    EXPECT_EQ("D7", subject->disassemble(7));
}