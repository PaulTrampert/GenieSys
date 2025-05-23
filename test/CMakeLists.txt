
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
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
        CpuOperations/ABCDTest.cpp
        CpuOperations/ADDTest.cpp
        GetPossibleOpcodesTest.cpp
        CpuOperations/ORItoCCRTest.cpp
        CpuOperations/CpuOperationTest.cpp
        CpuOperations/ORItoSRTest.cpp
        CpuOperations/ANDITest.cpp
        CpuOperations/ANDItoCCRTest.cpp
        CpuOperations/ANDItoSRTest.cpp
        CpuOperations/SUBITest.cpp
        CpuOperations/ADDITest.cpp
        CpuOperations/ORITest.cpp
        CpuOperations/EORItoCCRTest.cpp
        CpuOperations/EORItoSRTest.cpp
        CpuOperations/EORITest.cpp
        CpuOperations/CMPITest.cpp
        CpuOperations/BTSTTest.cpp
        CpuOperations/BCHGTest.cpp
        CpuOperations/BCLRTest.cpp
        CpuOperations/BSETTest.cpp
        CpuOperations/MOVEPTest.cpp
        CpuOperations/MOVEATest.cpp
        CpuOperations/MOVETest.cpp
        CpuOperations/MOVEfromSRTest.cpp
        CpuOperations/MOVEtoCCRTest.cpp
        CpuOperations/MOVEtoSRTest.cpp
        CpuOperations/NEGXTest.cpp
        CpuOperations/CLRTest.cpp
        CpuOperations/NEGTest.cpp
        CpuOperations/NOTTest.cpp
        CpuOperations/EXTTest.cpp
        getCcrFlags.cpp
        CpuOperations/NBCDTest.cpp
        CpuOperations/SWAPTest.cpp
        CpuOperations/PEATest.cpp
        CpuOperations/ILLEGALTest.cpp
        CpuOperations/TASTest.cpp
        CpuOperations/TSTTest.cpp
        CpuOperations/TRAPTest.cpp
        CpuOperations/LINKTest.cpp
        CpuOperations/UNLKTest.cpp
        CpuOperations/MOVE_USPTest.cpp
        CpuOperations/RESETTest.cpp
        M68kCpuTests/SetDataRegisterTests.cpp
        M68kCpuTests/ResetTests.cpp
        CpuOperations/STOPTest.cpp
        CpuOperations/RTETest.cpp
        CpuOperations/RTRTest.cpp
        CpuOperations/RTSTest.cpp
        CpuOperations/TRAPVTest.cpp
        CpuOperations/JSRTest.cpp
        CpuOperations/JMPTest.cpp
        CpuOperations/MOVEMTest.cpp
        RegisterListTest.cpp
        CpuOperations/MOVEQTest.cpp
        CpuOperations/BccTest.cpp
        CpuOperations/BSRTest.cpp
        CpuOperations/BRATest.cpp
        M68kCpuTests/TestConditionCodeTests.cpp
        CpuOperations/DBccTest.cpp
        CpuOperations/SccTest.cpp
        CpuOperations/SUBQTest.cpp
)

target_link_libraries(testGenieSys PRIVATE GenieSys gtest gmock)

gtest_discover_tests(testGenieSys)