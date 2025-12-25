cmake_minimum_required(VERSION 3.5.0)
project(Chip8-Emulator VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Chip8-Emulator main.cpp)

include(CTest)
enable_testing()

