cmake_minimum_required(VERSION 3.16)
project(zrv32 LANGUAGES CXX)

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES src/**.cpp)
file(GLOB_RECURSE HEADER_FILES include/**.hpp)

add_executable(zrv32 ${SRC_FILES} ${HEADER_FILES})
target_include_directories(zrv32 PRIVATE include)
target_link_libraries(zrv32 PRIVATE glfw raylib winmm)

set_target_properties(zrv32 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
