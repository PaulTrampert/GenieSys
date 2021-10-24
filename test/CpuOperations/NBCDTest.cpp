//
// Created by pault on 10/23/2021.
//
#include <GenieSys/CpuOperations/NBCD.h>
#include <gtest/gtest.h>
#include <GenieSys/Bus.h>


class NBCDTest : public ::testing::Test {
public:
    GenieSys::NBCD* subject;
    GenieSys::Bus bus;
    GenieSys::M68kCpu* cpu;

    uint16_t opWord = 0b0100100000000000;

    NBCDTest() : Test() {
        cpu = bus.getCpu();
        subject = new GenieSys::NBCD(cpu, &bus);
    }

    ~NBCDTest() override {
        delete subject;
    }
};

TEST_F(NBCDTest, Disassemble) {
    ASSERT_EQ("NBCD D0", subject->disassemble(opWord));
}

TEST_F(NBCDTest, NegateNormalBcd) {
    cpu->setDataRegister(0, (uint8_t)0x15);
    ASSERT_EQ(6, subject->execute(opWord));
    ASSERT_EQ(0x85, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_EXTEND|GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(NBCDTest, NegateBcdZero) {
    cpu->setDataRegister(0, (uint8_t)0x00);
    ASSERT_EQ(6, subject->execute(opWord));
    ASSERT_EQ(0x00, cpu->getDataRegister(0));
    ASSERT_EQ(0, cpu->getCcrFlags());
}

TEST_F(NBCDTest, NegateNormalBcdWithExtend) {
    cpu->setDataRegister(0, (uint8_t)0x15);
    cpu->setCcrFlags(GenieSys::CCR_EXTEND);
    ASSERT_EQ(6, subject->execute(opWord));
    ASSERT_EQ(0x84, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_EXTEND|GenieSys::CCR_CARRY, cpu->getCcrFlags());
}

TEST_F(NBCDTest, NegateBcdZeroWithExtend) {
    cpu->setDataRegister(0, (uint8_t)0x00);
    cpu->setCcrFlags(GenieSys::CCR_EXTEND);
    ASSERT_EQ(6, subject->execute(opWord));
    ASSERT_EQ(0x99, cpu->getDataRegister(0));
    ASSERT_EQ(GenieSys::CCR_EXTEND|GenieSys::CCR_CARRY, cpu->getCcrFlags());
}