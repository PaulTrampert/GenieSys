set(GLOB HEADER_LIST "${GenieSys_SOURCE_DIR}/include/GenieSys/*.h")

add_library(libGenieSys
        Bus.cpp
        M68kCpu.cpp
        BriefExtensionWord.cpp
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
        AddressingModes/ImmediateDataMode.cpp)

target_include_directories(libGenieSys PUBLIC ../include)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)