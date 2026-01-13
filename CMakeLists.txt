cmake_minimum_required(VERSION 3.16)
project(zrv32 LANGUAGES CXX)

file(GLOB_RECURSE SRC_FILES src/**.cpp)
file(GLOB_RECURSE HEADER_FILES include/**.hpp)

add_executable(zrv32 ${SRC_FILES} ${HEADER_FILES})
target_include_directories(zrv32 PRIVATE include)

set_target_properties(zrv32 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
