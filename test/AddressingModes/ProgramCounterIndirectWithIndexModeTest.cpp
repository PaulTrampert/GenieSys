//
// Created by paul.trampert on 12/3/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectWithIndexMode.h>
#include <GenieSys/ExtensionWord.h>

struct ProgramCounterIndirectWithIndexModeTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    ProgramCounterIndirectWithIndexMode* subject;

    ProgramCounterIndirectWithIndexModeTest() {
        cpu = bus.getCpu();
        subject = new ProgramCounterIndirectWithIndexMode(cpu, &bus);
    }

    ~ProgramCounterIndirectWithIndexModeTest() override {
        delete subject;
    }
};

TEST_F(ProgramCounterIndirectWithIndexModeTest, ItHasTheCorrectModeId) {
    ASSERT_EQ(0b011u, subject->getModeId());
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_With8BitDisplacement_GetsTheCorrectAddress) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(true);
    extWord.setDisplacement(6);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(1);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(48, subject->getAddress(5));
    ASSERT_EQ(34, cpu->getPc());
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_With8BitDisplacement_DisassemblesCorrectly) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(true);
    extWord.setDisplacement(6);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(1);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("(#6,PC,D4.w*2)", subject->disassemble(5, 0));
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_EverythingSuppressed_GetsTheCorrectAddress) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(1);
    extWord.setIndexSuppress(true);
    extWord.setBaseRegSuppress(true);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_NULL);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(0, subject->getAddress(5));
    ASSERT_EQ(34, cpu->getPc());
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_EverythingSuppressed_Disassemble) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(1);
    extWord.setIndexSuppress(true);
    extWord.setBaseRegSuppress(true);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_NULL);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("(,,)", subject->disassemble(5, 0));
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_OnlyProgramCounter_GetsTheCorrectAddress) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(1);
    extWord.setIndexSuppress(true);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_NULL);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(32, subject->getAddress(5));
    ASSERT_EQ(34, cpu->getPc());
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_OnlyProgramCounter_Disassemble) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(1);
    extWord.setIndexSuppress(true);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_NULL);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("(,PC,)", subject->disassemble(5, 0));
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_PcWithDisplacement_GetsTheCorrectAddress) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(1);
    extWord.setIndexSuppress(true);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_WORD);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    bus.writeWord(34, -4);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(28, subject->getAddress(5));
    ASSERT_EQ(36, cpu->getPc());
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_PcWithDisplacement_Disassemble) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(1);
    extWord.setIndexSuppress(true);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_WORD);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    bus.writeWord(34, -4);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("(#-4,PC,)", subject->disassemble(5, 0));
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_PcWithIndex_GetsTheCorrectAddress) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(2);
    extWord.setIndexSuppress(false);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_NULL);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(52, subject->getAddress(5));
    ASSERT_EQ(34, cpu->getPc());
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_PcWithIndex_Disassemble) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(2);
    extWord.setIndexSuppress(false);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_NULL);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("(,PC,D4.w*4)", subject->disassemble(5, 0));
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_PcWithIndexAndDisplacement_GetsTheCorrectAddress) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(2);
    extWord.setIndexSuppress(false);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_WORD);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    bus.writeWord(34, -4);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(48, subject->getAddress(5));
    ASSERT_EQ(36, cpu->getPc());
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_PcWithIndexAndDisplacement_Disassemble) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(2);
    extWord.setIndexSuppress(false);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_WORD);
    extWord.setIndexIndirectSelection(0);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    bus.writeWord(34, -4);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("(#-4,PC,D4.w*4)", subject->disassemble(5, 0));
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_PcPreIndexedWithOuterDisplacement_GetsTheCorrectAddress) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(2);
    extWord.setIndexSuppress(false);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_WORD);
    extWord.setIndexIndirectSelection(2);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    bus.writeWord(34, -2);
    bus.writeWord(36, -8);
    bus.writeLong(50, 9000);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(8992, subject->getAddress(5));
    ASSERT_EQ(38, cpu->getPc());
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_PcPreIndexedWithOuterDisplacement_Disassemble) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(2);
    extWord.setIndexSuppress(false);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_WORD);
    extWord.setIndexIndirectSelection(2);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    bus.writeWord(34, -2);
    bus.writeWord(36, -8);
    bus.writeLong(50, 9000);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("([#-2,PC,D4.w*4],#-8)", subject->disassemble(5, 0));
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_AddressRegisterPostIndexedWithOuterDisplacement_GetsTheCorrectAddress) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(2);
    extWord.setIndexSuppress(false);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_WORD);
    extWord.setIndexIndirectSelection(6);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    bus.writeWord(34, -4);
    bus.writeWord(36, -8);
    bus.writeLong(28, 9000);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(9012, subject->getAddress(5));
    ASSERT_EQ(38, cpu->getPc());
}

TEST_F(ProgramCounterIndirectWithIndexModeTest, GetAddress_AddressRegisterPostIndexedWithOuterDisplacement_Disassemble) {
    ExtensionWord extWord = ExtensionWord();
    extWord.setIsBrief(false);
    extWord.setIdxRegType(M68K_REG_TYPE_DATA);
    extWord.setIdxRegAddr(4);
    extWord.setIdxSize(EXT_WORD_IDX_SIZE_SE_WORD);
    extWord.setScale(2);
    extWord.setIndexSuppress(false);
    extWord.setBaseDisplacementSize(EXT_WORD_BD_SIZE_WORD);
    extWord.setIndexIndirectSelection(6);
    cpu->setPc(32);
    bus.writeWord(32, (uint16_t)extWord);
    bus.writeWord(34, -4);
    bus.writeWord(36, -8);
    bus.writeLong(28, 9000);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("([#-4,PC],D4.w*4,#-8)", subject->disassemble(5, 0));
}