
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
        BriefExtensionWordTest.cpp
        ExtensionWordTest.cpp
        AddressingModes/AbsoluteLongAddressingModeTest.cpp
        AddressingModes/AbsoluteShortAddressingModeTest.cpp)

target_link_libraries(testGenieSys PRIVATE libGenieSys gtest gmock)

gtest_discover_tests(testGenieSys)