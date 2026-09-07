//
// Created by paul on 9/6/25.
//

#pragma once

#include <cstdint>

namespace GenieSys {
    /**
     * Index into the per addressing mode timing tables shared by the control instructions
     * JMP, JSR, LEA and PEA. Those instructions are documented with a complete execution time
     * for each addressing mode rather than a base plus an effective address calculation, so
     * they index a table with this rather than adding AddressingResult::getCycles().
     */
    enum CONTROL_MODE_INDEX {
        CTRL_MODE_AN_INDIRECT = 0,      // (An)
        CTRL_MODE_AN_DISPLACEMENT = 1,  // d(An)
        CTRL_MODE_AN_INDEX = 2,         // d(An,Xn)
        CTRL_MODE_ABS_SHORT = 3,        // (xxx).W
        CTRL_MODE_ABS_LONG = 4,         // (xxx).L
        CTRL_MODE_PC_DISPLACEMENT = 5,  // d(PC)
        CTRL_MODE_PC_INDEX = 6,         // d(PC,Xn)
        CTRL_MODE_COUNT = 7,            // number of control addressing modes
        CTRL_MODE_INVALID = 8           // not a control addressing mode
    };

    /**
     * Classify an effective address as one of the control addressing modes.
     *
     * @param eaModeId The effective address mode field.
     * @param eaReg    The effective address register field, which selects the sub mode when
     *                 eaModeId is 0b111.
     * @return The matching CONTROL_MODE_INDEX, or CTRL_MODE_INVALID when the effective address
     *         is not a control addressing mode and so cannot be used by JMP, JSR, LEA or PEA.
     */
    CONTROL_MODE_INDEX getControlModeIndex(uint8_t eaModeId, uint8_t eaReg);
}
