//
// Created by paul on 8/18/25.
//
#include <vector>
#include <sstream>
#include <GenieSys/CpuOperations/CHK.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>


GenieSys::CHK::CHK(M68kCpu* cpu, Bus* bus) : CpuOperation(cpu, bus)
{
}

std::string GenieSys::CHK::disassemble(uint16_t opWord)
{
    std::stringstream disassembly;
    const uint8_t dn = dnMask.apply(opWord);
    const uint8_t eaMode = eaModeMask.apply(opWord);
    const uint8_t eaAddr = eaAddrMask.apply(opWord);
    auto eaAddrMode = cpu->getAddressingMode(eaMode);
    auto dnAddrMode = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);

    disassembly << "CHK " << eaAddrMode->disassemble(eaAddr, 2) << ", " << dnAddrMode->disassemble(dn, 2);

    return disassembly.str();
}

uint8_t GenieSys::CHK::execute(uint16_t opWord)
{
    uint8_t dn = dnMask.apply(opWord);
    const uint8_t eaMode = eaModeMask.apply(opWord);
    const uint8_t eaAddr = eaAddrMask.apply(opWord);
    auto eaAddrMode = cpu->getAddressingMode(eaMode);
    auto dnAddrMode = cpu->getAddressingMode(GenieSys::DataRegisterDirectMode::MODE_ID);
    auto eaResult = eaAddrMode->getData(eaAddr, 2);
    auto dnResult = dnAddrMode->getData(dn, 2);
    auto eaValue = static_cast<int16_t>(eaResult->getDataAsWord());
    auto dnValue = static_cast<int16_t>(dnResult->getDataAsWord());

    if (dnValue < 0 || dnValue > eaValue)
    {
        auto ccr = cpu->getCcrFlags();
        if (dnValue < 0)
            ccr |= CCR_NEGATIVE;
        else
            ccr &= ~CCR_NEGATIVE;
        cpu->setCcrFlags(ccr);
        return cpu->trap(TV_CHK);
    }
    return 10;
}

std::vector<uint16_t> GenieSys::CHK::getOpcodes()
{
    return getPossibleOpcodes(baseOpWord, std::vector{
        &dnMask,
        &sizeMask,
        &eaModeMask,
        &eaAddrMask
    });
}

uint8_t GenieSys::CHK::getSpecificity()
{
    return dnMask.getWidth() + sizeMask.getWidth() + eaModeMask.getWidth() + eaAddrMask.getWidth();
}
