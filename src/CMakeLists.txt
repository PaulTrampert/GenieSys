set(GLOB HEADER_LIST "${GenieSys_SOURCE_DIR}/include/GenieSys/*.h")

add_library(libGenieSys Bus.cpp M68kCpu.cpp BriefExtensionWord.cpp ExtensionWord.cpp)

target_include_directories(libGenieSys PUBLIC ../include)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)