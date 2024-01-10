cmake_minimum_required(VERSION 3.27)
project(ProjetChipEight C)

set(CMAKE_C_STANDARD 23)

find_package(SDL2 REQUIRED)

add_executable(ProjetChipEight main.c
        hardware/memory.h
        hardware/memory.c
        hardware/processor.h
        hardware/processor.c
        chip8.c
        chip8.h
)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
target_link_libraries(ProjetChipEight ${SDL2_LIBRARIES}
/home/mlanglois/Documents/ProjetChipEight/libprovided/lib/release/x64/libprovided.a)
