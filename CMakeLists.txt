cmake_minimum_required(VERSION 3.27)
project(ProjetChipEight C)

set(CMAKE_C_STANDARD 23)

add_executable(ProjetChipEight main.c
        memory.h
        memory.c
)
