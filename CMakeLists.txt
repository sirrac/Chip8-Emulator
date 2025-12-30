cmake_minimum_required(VERSION 3.5.0)
project(Chip8-Emulator VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_PREFIX_PATH "C:/Users/srika/Downloads/SDL3-devel-3.2.28-mingw/SDL3-3.2.28/cmake")
find_package(SDL3 REQUIRED)

add_executable(Chip8-Emulator main.cpp Chip8.cpp)

target_link_libraries(Chip8-Emulator PRIVATE SDL3::SDL3)

include(CTest)
enable_testing()

