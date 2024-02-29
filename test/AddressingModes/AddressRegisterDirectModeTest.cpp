//
// Created by paul.trampert on 11/29/2020.
//
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <GenieSys/TrapException.h>


struct AddressRegisterDirectModeTest : testing::Test {
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;
    GenieSys::AddressRegisterDirectMode* subject;

    AddressRegisterDirectModeTest() {
        cpu = bus.getCpu();
        cpu->setAddressRegister(3, 9999);
        subject = new GenieSys::AddressRegisterDirectMode(cpu, &bus);
    }

    ~AddressRegisterDirectModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterDirectModeTest, ItGetsTheCorrectAddress) {
    EXPECT_EQ(3, subject->getAddress(3));
}

TEST_F(AddressRegisterDirectModeTest, ItGetsTheCorrectData) {
    std::vector<uint8_t> result = subject->getData(3, 4)->getData();
    uint32_t finalResult = 0;
    for (int i = 0; i < 4; i++) {
        finalResult |= result[i] << (8 * (3 - i));
    }
    EXPECT_EQ(9999, finalResult);
}

TEST_F(AddressRegisterDirectModeTest, TestGetModeId) {
    EXPECT_EQ(0b001u, subject->getModeId());
}

TEST_F(AddressRegisterDirectModeTest, TestDisassemble) {
    EXPECT_EQ("A7", subject->disassemble(7, 0));
}

TEST_F(AddressRegisterDirectModeTest, TestMovemToReg) {
    EXPECT_THROW({
        try {
            subject->movemToReg(1, 2, 1);
        } catch (GenieSys::TrapException &e) {
            EXPECT_EQ(GenieSys::TV_ILLEGAL_INSTR, e.getTrapVector());
            throw;
        }
    }, GenieSys::TrapException);
}

TEST_F(AddressRegisterDirectModeTest, TestMovemToMem) {
    EXPECT_THROW({
                     try {
                         subject->movemToMem(1, 2, 1);
                     } catch (GenieSys::TrapException &e) {
                         EXPECT_EQ(GenieSys::TV_ILLEGAL_INSTR, e.getTrapVector());
                         throw;
                     }
                 }, GenieSys::TrapException);
}