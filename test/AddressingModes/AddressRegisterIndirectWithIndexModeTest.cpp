//
// Created by paul.trampert on 12/2/2020.
//

#include <gtest/gtest.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectWithIndexMode.h>
#include <GenieSys/ExtensionWord.h>

struct AddressRegisterIndirectWithIndexModeTest : testing::Test {
    M68kCpu* cpu;
    Bus bus;
    AddressRegisterIndirectWithIndexMode* subject;

    AddressRegisterIndirectWithIndexModeTest() {
        cpu = bus.getCpu();
        subject = new AddressRegisterIndirectWithIndexMode(cpu, &bus);
    }

    ~AddressRegisterIndirectWithIndexModeTest() override {
        delete subject;
    }
};

TEST_F(AddressRegisterIndirectWithIndexModeTest, ItHasTheCorrectModeId) {
    ASSERT_EQ(0b110u, subject->getModeId());
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_With8BitDisplacement_GetsTheCorrectAddress) {
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

    ASSERT_EQ(110, subject->getAddress(5));
    ASSERT_EQ(34, cpu->getPc());
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_With8BitDisplacement_DisassemblesCorrectly) {
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

    ASSERT_EQ("(#6,A5,D4.w*2)", subject->disassemble(5, 0));
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_EverythingSuppressed_GetsTheCorrectAddress) {
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

    ASSERT_EQ(0, subject->getAddress(5));
    ASSERT_EQ(34, cpu->getPc());
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_EverythingSuppressed_Disassemble) {
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

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_OnlyAddressRegister_GetsTheCorrectAddress) {
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

    ASSERT_EQ(94, subject->getAddress(5));
    ASSERT_EQ(34, cpu->getPc());
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_OnlyAddressRegister_Disassemble) {
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

    ASSERT_EQ("(,A5,)", subject->disassemble(5, 0));
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressWithDisplacement_GetsTheCorrectAddress) {
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
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(90, subject->getAddress(5));
    ASSERT_EQ(36, cpu->getPc());
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressWithDisplacement_Disassemble) {
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
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("(#-4,A5,)", subject->disassemble(5, 0));
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressRegisterWithIndex_GetsTheCorrectAddress) {
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
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(114, subject->getAddress(5));
    ASSERT_EQ(34, cpu->getPc());
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressRegisterWithIndex_Disassemble) {
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
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("(,A5,D4.w*4)", subject->disassemble(5, 0));
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressRegisterWithIndexAndDisplacement_GetsTheCorrectAddress) {
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
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(110, subject->getAddress(5));
    ASSERT_EQ(36, cpu->getPc());
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressRegisterWithIndexAndDisplacement_Disassemble) {
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
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("(#-4,A5,D4.w*4)", subject->disassemble(5, 0));
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressRegisterPreIndexedWithOuterDisplacement_GetsTheCorrectAddress) {
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
    bus.writeWord(34, -4);
    bus.writeWord(36, -4);
    bus.writeLong(110, 9000);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(8996, subject->getAddress(5));
    ASSERT_EQ(38, cpu->getPc());

}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressRegisterPreIndexedWithOuterDisplacement_Disassemble) {
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
    bus.writeWord(34, -4);
    bus.writeWord(36, -4);
    bus.writeLong(110, 9000);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("([#-4,A5,D4.w*4],#-4)", subject->disassemble(5, 0));

}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressRegisterPostIndexedWithOuterDisplacement_GetsTheCorrectAddress) {
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
    bus.writeWord(36, -4);
    bus.writeLong(90, 9000);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ(9016, subject->getAddress(5));
    ASSERT_EQ(38, cpu->getPc());
}

TEST_F(AddressRegisterIndirectWithIndexModeTest, GetAddress_AddressRegisterPostIndexedWithOuterDisplacement_Disassemble) {
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
    bus.writeWord(36, -4);
    bus.writeLong(90, 9000);
    cpu->setAddressRegister(5, 94);
    cpu->setDataRegister(4, (uint32_t)5);

    ASSERT_EQ("([#-4,A5],D4.w*4,#-4)", subject->disassemble(5, 0));
}