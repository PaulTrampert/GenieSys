set(GLOB HEADER_LIST "${GenieSys_SOURCE_DIR}/include/GenieSys/*.h")

add_library(GenieSys
        Bus.cpp
        M68kCpu.cpp
        ExtensionWord.cpp
        AddressingModes/DataRegisterDirectMode.cpp
        AddressingModes/AddressingMode.cpp
        AddressingModes/AddressRegisterDirectMode.cpp
        AddressingModes/AddressRegisterIndirectMode.cpp
        AddressingModes/AddressRegisterIndirectPostIncrementMode.cpp
        AddressingModes/AddressRegisterIndirectPreDecrementMode.cpp
        AddressingModes/AddressRegisterIndirectDisplacementMode.cpp
        AddressingModes/AddressRegisterIndirectWithIndexMode.cpp
        AddressingModes/ProgramCounterIndirectDisplacementMode.cpp
        AddressingModes/ProgramCounterIndirectWithIndexMode.cpp
        AddressingModes/AbsoluteShortAddressingMode.cpp
        AddressingModes/AbsoluteLongAddressingMode.cpp
        AddressingModes/ImmediateDataMode.cpp
        AddressingModes/ProgramCounterAddressingMode.cpp
        numberUtils.cpp
        CpuOperations/ABCD.cpp
        CpuOperations/CpuOperation.cpp
        CpuOperations/ADD.cpp
        CpuOperations/NOP.cpp
        CpuOperations/ORItoCCR.cpp
        CpuOperations/ORItoSR.cpp
        CpuOperations/ANDI.cpp
        CpuOperations/ANDItoCCR.cpp
        CpuOperations/ANDItoSR.cpp
        CpuOperations/SUBI.cpp
        CpuOperations/ADDI.cpp
        CpuOperations/ORI.cpp
        CpuOperations/EORItoCCR.cpp
        CpuOperations/EORItoSR.cpp
        CpuOperations/EORI.cpp
        CpuOperations/CMPI.cpp
        CpuOperations/BTST.cpp
        CpuOperations/BCHG.cpp
        CpuOperations/BCLR.cpp
        CpuOperations/BSET.cpp
        CpuOperations/MOVEP.cpp
        CpuOperations/MOVEA.cpp
        CpuOperations/MOVE.cpp
        CpuOperations/MOVEfromSR.cpp
        CpuOperations/MOVEtoCCR.cpp
        CpuOperations/MOVEtoSR.cpp
        CpuOperations/NEGX.cpp
        CpuOperations/CLR.cpp
        CpuOperations/NEG.cpp
        CpuOperations/NOT.cpp
        CpuOperations/EXT.cpp
        CpuOperations/NBCD.cpp
        CpuOperations/SWAP.cpp
        CpuOperations/PEA.cpp
        CpuOperations/ILLEGAL.cpp
        CpuOperations/TAS.cpp
        CpuOperations/TST.cpp
        CpuOperations/TRAP.cpp
        CpuOperations/LINK.cpp
        CpuOperations/UNLK.cpp
        CpuOperations/MOVE_USP.cpp CpuOperations/RESET.cpp)

target_include_directories(GenieSys PUBLIC ../include)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)

OPTION(ENABLE_DISASSEMBLY "Enables disassembly when running the emulation" ON)
IF(ENABLE_DISASSEMBLY)
    ADD_DEFINITIONS(-DDISASSEMBLE)
ENDIF(ENABLE_DISASSEMBLY)