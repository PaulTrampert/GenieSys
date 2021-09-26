set(GLOB HEADER_LIST "${GenieSys_SOURCE_DIR}/include/GenieSys/*.h")

add_library(libGenieSys
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
        CpuOperations/Abcd.cpp
        CpuOperations/CpuOperation.cpp
        CpuOperations/Add.cpp
        CpuOperations/Nop.cpp
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
        CpuOperations/MOVE.cpp)

target_include_directories(libGenieSys PUBLIC ../include)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)

OPTION(ENABLE_DISASSEMBLY "Enables disassembly when running the emulation" ON)
IF(ENABLE_DISASSEMBLY)
    ADD_DEFINITIONS(-DDISASSEMBLE)
ENDIF(ENABLE_DISASSEMBLY)