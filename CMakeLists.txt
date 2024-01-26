cmake_minimum_required(VERSION 3.22.1)
project(ProjetChipEight C)

set(CMAKE_C_STANDARD 17)
find_package(SDL2 REQUIRED)

add_executable(ProjetChipEight main.c
        chip8.c
        chip8.h
        hardware/memory.h
        hardware/memory.c
        hardware/processor.h
        hardware/processor.c
)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/libprovided/include)
target_link_libraries(ProjetChipEight ${SDL2_LIBRARIES}
        ${CMAKE_SOURCE_DIR}/libprovided/lib/release/x64/libprovided.a
        -lm)
