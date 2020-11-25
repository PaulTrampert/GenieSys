
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)

add_executable(testGenieSys main.cpp BusTest.cpp signExtendTest.cpp)

target_link_libraries(testGenieSys PRIVATE libGenieSys gtest)

