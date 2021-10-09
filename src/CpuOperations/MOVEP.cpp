//
// Created by pault on 9/14/2021.
//
#include <GenieSys/CpuOperations/MOVEP.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <vector>
#include <GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h>
#include <sstream>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>



MOVEP::MOVEP(GenieSys::M68kCpu *cpu, Bus *bus) : CpuOperation(cpu, bus) {

}



std::string MOVEP::disassemble(uint16_t opWord) {
    auto dn = DnMask.apply(opWord);
    auto opMode = (OpMode)OpModeMask.apply(opWord);
    auto dnMode = cpu->getAddressingMode(DataRegisterDirectMode::MODE_ID);
    auto addrMode = cpu->getAddressingMode(AddressRegisterIndirectDisplacementMode::MODE_ID);
    auto an = AnMask.apply(opWord);
    std::stringstream stream;
    stream << "MOVEP";
    if (opMode == LONG_REG_MEM) {
        stream << ".l " << dnMode->disassemble(dn, 4) << "," << addrMode->disassemble(an, 4);
    }
    else if (opMode == WORD_REG_MEM) {
        stream << ".w " << dnMode->disassemble(dn, 2) << "," << addrMode->disassemble(an, 2);
    }
    else if (opMode == LONG_MEM_REG) {
        stream << ".l " << addrMode->disassemble(an, 4) << "," << dnMode->disassemble(dn, 4);
    }
    else if (opMode == WORD_MEM_REG) {
        stream << ".w " << addrMode->disassemble(an, 2) << "," << dnMode->disassemble(dn, 2);
    }

    return stream.str();
}

uint8_t MOVEP::execute(uint16_t opWord) {
    auto dn = DnMask.apply(opWord);
    auto opMode = (OpMode)OpModeMask.apply(opWord);
    auto addrMode = cpu->getAddressingMode(AddressRegisterIndirectDisplacementMode::MODE_ID);
    auto an = AnMask.apply(opWord);
    auto anAddr = addrMode->getAddress(an);
    switch (opMode) {
        case LONG_MEM_REG:
            LongMemToReg(anAddr, dn);
            break;
        case LONG_REG_MEM:
            LongRegToMem(anAddr, dn);
            break;
        case WORD_MEM_REG:
            WordMemToReg(anAddr, dn);
            break;
        case WORD_REG_MEM:
            WordRegToMem(anAddr, dn);
            break;
    }
    return opMode == LONG_REG_MEM || opMode == LONG_MEM_REG ? 24 : 16;
}

std::vector<uint16_t> MOVEP::getOpcodes() {
    return getPossibleOpcodes((uint16_t) 0b0000000000001000, std::vector<GenieSys::BitMask<uint16_t>*> {
        &DnMask,
        &AnMask,
        &OpModeMask
    });
}

uint8_t MOVEP::getSpecificity() {
    return DnMask.getWidth() + AnMask.getWidth() + OpModeMask.getWidth();
}

void MOVEP::WordMemToReg(uint32_t anAddr, uint8_t dn) {
    uint16_t dnData = ((uint16_t) bus->read(anAddr) << 8) + bus->read(anAddr + 2);
    cpu->setDataRegister(dn, dnData);
}

void MOVEP::LongMemToReg(uint32_t anAddr, uint8_t dn) {
    uint32_t dnData = 0;
    for (int i = 0; i < 4; i++) {
        uint32_t shift = (3 - i) * 8;
        uint32_t readAddr = anAddr + (2 * i);
        dnData |= (bus->read(readAddr) << shift);
    }
    cpu->setDataRegister(dn, dnData);
}

void MOVEP::WordRegToMem(uint32_t anAddr, uint8_t dn) {
    uint16_t dnData = (cpu->getDataRegister(dn) & 0x0000FFFF);
    bus->writeByte(anAddr, ((dnData & 0xFF00) >> 8));
    bus->writeByte(anAddr + 2, (dnData & 0x00FF));
}

void MOVEP::LongRegToMem(uint32_t anAddr, uint8_t dn) {
    uint32_t dnData = cpu->getDataRegister(dn);
    for (int i = 0; i < 4; i++) {
        uint32_t shift = (3 - i) * 8;
        uint32_t mask = (0xFFu) << shift;
        uint32_t writeAddr = anAddr + (2 * i);
        bus->writeByte(writeAddr, (dnData & mask) >> shift);
    }
}
