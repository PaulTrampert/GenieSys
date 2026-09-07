//
// Created by paul on 9/6/25.
//

#include <GenieSys/getControlModeIndex.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/AddressRegisterIndirectWithIndexMode.h>
#include <GenieSys/AddressingModes/AbsoluteShortAddressingMode.h>
#include <GenieSys/AddressingModes/AbsoluteLongAddressingMode.h>
#include <GenieSys/AddressingModes/ProgramCounterAddressingMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectDisplacementMode.h>
#include <GenieSys/AddressingModes/ProgramCounterIndirectWithIndexMode.h>

GenieSys::CONTROL_MODE_INDEX GenieSys::getControlModeIndex(uint8_t eaModeId, uint8_t eaReg) {
    switch (eaModeId) {
        case GenieSys::AddressRegisterIndirectMode::MODE_ID:
            return GenieSys::CTRL_MODE_AN_INDIRECT;
        case GenieSys::AddressRegisterIndirectDisplacementMode::MODE_ID:
            return GenieSys::CTRL_MODE_AN_DISPLACEMENT;
        case GenieSys::AddressRegisterIndirectWithIndexMode::MODE_ID:
            return GenieSys::CTRL_MODE_AN_INDEX;
        case GenieSys::ProgramCounterAddressingMode::MODE_ID:
            switch (eaReg) {
                case GenieSys::AbsoluteShortAddressingMode::MODE_ID:
                    return GenieSys::CTRL_MODE_ABS_SHORT;
                case GenieSys::AbsoluteLongAddressingMode::MODE_ID:
                    return GenieSys::CTRL_MODE_ABS_LONG;
                case GenieSys::ProgramCounterIndirectDisplacementMode::MODE_ID:
                    return GenieSys::CTRL_MODE_PC_DISPLACEMENT;
                case GenieSys::ProgramCounterIndirectWithIndexMode::MODE_ID:
                    return GenieSys::CTRL_MODE_PC_INDEX;
                default:
                    return GenieSys::CTRL_MODE_INVALID;
            }
        default:
            // Dn, An, (An)+ and -(An) are all data or alterable modes only.
            return GenieSys::CTRL_MODE_INVALID;
    }
}
