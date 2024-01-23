cmake_minimum_required(VERSION 3.27)
project(ProjetChipEight C)

set(CMAKE_C_STANDARD 17)

find_package(SDL2 REQUIRED)

add_executable(ProjetChipEight main.c
        hardware/memory.h
        hardware/memory.c
        hardware/processor.h
        hardware/processor.c
        chip8.c
        chip8.h
        libprovided/include/display/display.h
        libprovided/include/display/sprite.h
        libprovided/include/keyboard/keyboard.h
        libprovided/include/misc/error.h
        libprovided/include/speaker/speaker.h
)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
target_link_libraries(ProjetChipEight ${SDL2_LIBRARIES}
/home/romain/CLionProjects/ProjetChipEight/libprovided/lib/release/x64/libprovided.a -lm)
