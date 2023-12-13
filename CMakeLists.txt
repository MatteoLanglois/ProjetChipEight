cmake_minimum_required(VERSION 3.27)
project(ProjetChipEight C)

set(CMAKE_C_STANDARD 23)

add_executable(ProjetChipEight main.c
        hardware/memory.h
        hardware/memory.c
        hardware/processor.h
        hardware/processor.c
        hardware/stack.c
        hardware/stack.h
        hardware/register.c
        hardware/register.h
        hardware/timer.c
        hardware/timer.h
)
