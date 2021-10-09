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


class Bus;
class AddressingMode;
class CpuOperation;

namespace GenieSys {
    /**
         * The CPU used in the Sega Genesis.
         */
    class M68kCpu {

    private:
        /* Private fields */
        /**
         * The 32-bit addressable bus.
         */
        Bus* bus = nullptr;

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
        /**
         * System stack pointer;
         */
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
        void ConnectBus(Bus* bus);
        uint16_t getCurrentOpWord();
        uint32_t getDataRegister(uint8_t addr);
        void setDataRegister(uint8_t addr, uint8_t value);
        void setDataRegister(uint8_t addr, uint16_t value);
        void setDataRegister(uint8_t addr, uint32_t value);
        uint32_t getAddressRegister(uint8_t addr);
        void setAddressRegister(uint8_t addr, uint32_t value);
        uint32_t getPc();
        void incrementPc(int32_t amount);
        void setPc(uint32_t value);
        void setCcrFlags(uint8_t ccr);
        uint8_t getCcrFlags();
        void tick();
        uint16_t getSR();
        void setSR(uint16_t sr);
        bool isSupervisor();
        void trap(uint8_t vector);

        AddressingMode *getAddressingMode(int modeId);
    };
}


