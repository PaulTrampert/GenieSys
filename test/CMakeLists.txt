
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(testGenieSys
        main.cpp
        BusTest.cpp
        signExtendTest.cpp
        ExtensionWordTest.cpp
        AddressingModes/AbsoluteLongAddressingModeTest.cpp
        AddressingModes/AbsoluteShortAddressingModeTest.cpp
        AddressingModes/AddressRegisterDirectModeTest.cpp
        AddressingModes/AddressRegisterIndirectDisplacementModeTest.cpp
        AddressingModes/AddressRegisterIndirectModeTest.cpp
        AddressingModes/AddressRegisterIndirectPostIncrementModeTest.cpp
        numberUtils.cpp
        AddressingModes/AddressRegisterIndirectPreDecrementModeTest.cpp
        AddressingModes/AddressRegisterIndirectWithIndexModeTest.cpp
        AddressingModes/DataRegisterDirectModeTest.cpp
        AddressingModes/ImmediateDataModeTest.cpp
        AddressingModes/ProgramCounterIndirectDisplacementModeTest.cpp
        AddressingModes/ProgramCounterIndirectWithIndexModeTest.cpp
        BitMaskTest.cpp
        CpuOperations/AbcdTest.cpp
        CpuOperations/AddTest.cpp
        GetPossibleOpcodesTest.cpp)

target_link_libraries(testGenieSys PRIVATE libGenieSys gtest gmock)

gtest_discover_tests(testGenieSys)