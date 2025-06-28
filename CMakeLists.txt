cmake_minimum_required(VERSION 3.31)
project(TraitRuntime C)

set(CMAKE_C_STANDARD 11)

add_executable(TraitRuntime main.c
        trait_runtime.c
        trait_runtime.h
        hashed_str.c
        hashed_str.h
)
