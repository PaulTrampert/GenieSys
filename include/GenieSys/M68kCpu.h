//
// Created by paul.trampert on 11/8/2020.
//

#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <memory>
#include "GenieSys/enums.h"
#include "./BitMask.h"

namespace GenieSys {
    class Bus;
    class AddressingMode;
    class CpuOperation;
    /**
     * The CPU used in the Sega Genesis.
     */
    class M68kCpu {

    private:
        /* Private fields */
        /**
         * The 32-bit addressable bus.
         */
        GenieSys::Bus* bus = nullptr;

        /**
         * Data registers
         */
        std::array<uint32_t, 8> dataRegisters {
            0, 0, 0, 0, 0, 0, 0, 0
        };

        /**
         * Address registers
         */
        std::array<uint32_t, 8> addressRegisters {
                0, 0, 0, 0, 0, 0, 0, 0
        };

        /**
         * Program counter.
         */
        uint32_t pc = 0;

        /**
         * Condition code register
         */
        uint16_t SRandCCR = 0;
        uint32_t ssp = 0;

        BitMask<uint16_t> supervisorState = BitMask<uint16_t>(13, 1);
        BitMask<uint16_t> masterInterruptState = BitMask<uint16_t>(12, 1);
        /**
         * Floating point data registers.
         */
        std::array<long double, 8> fpDataRegisters {
                0, 0, 0, 0, 0, 0, 0, 0
        };
        uint8_t fpExceptionEnableByte = 0;
        uint8_t fpModeControl = 0;
        uint8_t fpConditionCode = 0;
        uint8_t fpQuotient = 0;
        uint8_t fpExceptionStatus = 0;
        uint8_t fpAccruedException = 0;
        uint32_t fpInstructionAddrRegister = 0;



        /* Operational Fields */
        uint16_t opWord;  // Single Effective Address Operation Word

        uint8_t clock = 0;

        /* Addressing modes */
        std::array<std::unique_ptr<AddressingMode>, 8> addressingModes;

        std::vector<std::shared_ptr<CpuOperation>> opTable = std::vector<std::shared_ptr<CpuOperation>>(65536);

        std::shared_ptr<CpuOperation> nop;

    public:
        M68kCpu();
        ~M68kCpu();

        /**
         * Connects the cpu to the given bus.
         * @param bus The bus to connect to.
         */
        virtual void ConnectBus(Bus* bus);

        /**
         * Gets the currently loaded opWord
         * @return The current opWord
         */
        virtual uint16_t getCurrentOpWord();

        /**
         * Gets the 32-bit value stored in the specified data register.
         * @param addr The address of the data register.
         * @return The value stored in the register.
         */
        virtual uint32_t getDataRegister(uint8_t addr);

        /**
         * Sets the lowest byte of the specified data register.
         * @param addr The address of the register.
         * @param value The low byte to set.
         */
        virtual void setDataRegister(uint8_t addr, uint8_t value);

        /**
         * Sets the lowest 16-bit value of the specified data register.
         *
         * @param addr The address of the register.
         * @param value The low 16-bit value to set.
         */
        virtual void setDataRegister(uint8_t addr, uint16_t value);

        /**
         * Sets the value of the specified data register.
         *
         * @param addr The address of the register.
         * @param value The value to set.
         */
        virtual void setDataRegister(uint8_t addr, uint32_t value);

        /**
         * Get the address stored in the specified address register.
         * @param addr The address of the register
         * @return The address stored in the specified register.
         */
        virtual uint32_t getAddressRegister(uint8_t addr);

        /**
         * Set the stored address in the specified address register. A7 is
         * treated as the supervisor stack pointer when in supervisor mode.
         * @param addr The address of the register to set.
         * @param value The address to store in the register.
         */
        virtual void setAddressRegister(uint8_t addr, uint32_t value);

        /**
         * Gets the current value of the program counter.
         * @return The current value of the program counter.
         */
        virtual uint32_t getPc();

        /**
         * Increments the program counter by the specified amount.
         * @param amount The amount to increment the program counter.
         */
        virtual void incrementPc(int32_t amount);

        /**
         * Sets the program counter to the provided value.
         * @param value The new value of the program counter.
         */
        virtual void setPc(uint32_t value);

        /**
         * Sets the Condition Code Register (CCR) flags.
         * @param ccr The new CCR flags.
         */
        virtual void setCcrFlags(uint8_t ccr);

        /**
         * Gets the condition code register (CCR).
         * @return The value of the CCR
         */
        virtual uint8_t getCcrFlags();

        /**
         * Process one tick of the cpu clock.
         */
        virtual void tick();

        /**
         * Get the Status Register (SR). The lower 8-bits are the CCR.
         * @return The value of the SR.
         */
        virtual uint16_t getSR();

        /**
         * Set the full value of the SR.
         * @param sr The new value of the SR.
         */
        virtual void setSR(uint16_t sr);

        /**
         * Get whether or not the cpu is in supervisor mode.
         * @return True if supervisor, false otherwise.
         */
        virtual bool isSupervisor();

        /**
         * Set the supervisor state of the cpu.
         * @param isSupervisor The new supervisor state.
         */
        virtual void setSupervisor(bool isSupervisor);

        /**
         * Execute a TRAP. Switches the CPU into supervisor mode if
         * it isn't already, pushes the PC and SR onto the supervisor
         * stack, and reads the provided trap vector into the PC.
         * @param vector The trap vector to execute.
         * @return Returns 34, the cycle time for the TRAP instruction.
         */
        virtual uint8_t trap(uint8_t vector);

        /**
         * Gets the specified addressing mode.
         * @param modeId The id of the addressing mode.
         * @return The AddressingMode. Returned pointer should not be deleted.
         */
        virtual AddressingMode *getAddressingMode(int modeId);

        /**
         * Pushes a word onto the current stack.
         * @param value The 16-bit word to push onto the stack.
         */
        virtual void stackPush(uint16_t value);

        /**
         * Pushes a long onto the current stack.
         * @param value The 32-bit long to push onto the stack.
         */
        virtual void stackPush(uint32_t value);

        /**
         * Pops a word off of the stack.
         * @return The 16-bit word read off of the stack.
         */
        virtual uint16_t stackPopWord();

        /**
         * Pops a long off of the stack.
         * @return The 32-bit long read off of the stack.
         */
        virtual uint32_t stackPopLong();

        /**
         * Gets the address register used for the user stack
         * pointer.
         * @return 7
         */
        virtual uint8_t getUspRegister();

        /**
         * Explicitly get the USER stack pointer.
         * @return The value stored in the user stack pointer.
         */
        virtual uint32_t getUserStackPointer();

        /**
         * Explicitly set the USER stack pointer.
         * @param addr The value to put in the user stack pointer.
         */
        virtual void setUserStackPointer(uint32_t addr);

        /**
         * Get the current stack pointer. Will be SSP in supervisor mode.
         * @return The current stack pointer.
         */
        virtual uint32_t getStackPointer();

        /**
         * Set the current stack pointer. Will be the SSP in supervisor mode.
         * @param p The new value of the stack pointer.
         */
        virtual void setStackPointer(uint32_t p);

        /**
         * Resets the program counter to the value stored at 0x0004
         */
        virtual void reset();
    };
}


