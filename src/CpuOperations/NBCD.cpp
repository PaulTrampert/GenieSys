//
// Created by pault on 10/22/2021.
//

#include <GenieSys/CpuOperations/NBCD.h>
#include <GenieSys/getPossibleOpcodes.h>
#include <GenieSys/M68kCpu.h>
#include <GenieSys/AddressingModes/AddressingMode.h>
#include <vector>
#include <GenieSys/AddressingModes/DataRegisterDirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterDirectMode.h>
#include <sstream>

GenieSys::NBCD::NBCD(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) : CpuOperation(cpu, bus) {

}

uint8_t GenieSys::NBCD::getSpecificity() {
    return eaModeMask.getWidth() + eaRegMask.getWidth();
}

std::vector<uint16_t> GenieSys::NBCD::getOpcodes() {
    return getPossibleOpcodes(baseOpcode, std::vector<BitMask<uint16_t>*>{
        &eaModeMask,
        &eaRegMask
    });
}

uint8_t GenieSys::NBCD::execute(uint16_t opWord) {
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    auto eaResult = eaMode->getData(eaReg, 1);
    auto eaData = eaResult->getDataAsByte();
    auto oldCcr = cpu->getCcrFlags();
    auto x = (oldCcr & CCR_EXTEND) >> 4;
    auto tens = tensMask.apply(eaData);
    auto ones = onesMask.apply(eaData);
    tens = 10 - tens - (ones + x > 0 ? 1 : 0);
    ones = 10 - ones - x;
    if (tens == 10) {
        tens = 0;
    }
    if (ones == 10) {
        ones = 0;
    }
    eaData = tensMask.compose(0, tens) + onesMask.compose(0, ones);
    eaResult->write(eaData);
    uint8_t ccr;
    if (eaData == 0) {
        ccr = x == 1
                ? (CCR_EXTEND | CCR_CARRY | (oldCcr & CCR_ZERO))
                : (oldCcr & CCR_ZERO);
    }
    else {
        ccr = CCR_EXTEND | CCR_CARRY;
    }
    cpu->setCcrFlags(ccr);
    return (eaModeId == DataRegisterDirectMode::MODE_ID || eaModeId == AddressRegisterDirectMode::MODE_ID)
        ? 6
        : (8 + eaResult->getCycles());
}

std::string GenieSys::NBCD::disassemble(uint16_t opWord) {
    std::stringstream stream;
    uint8_t eaModeId = eaModeMask.apply(opWord);
    uint8_t eaReg = eaRegMask.apply(opWord);
    auto eaMode = cpu->getAddressingMode(eaModeId);
    stream << "NBCD " << eaMode->disassemble(eaReg, 1);
    return stream.str();
}
