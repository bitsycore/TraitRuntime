cmake_minimum_required(VERSION 3.22)
project(TraitRuntime C)

set(CMAKE_C_STANDARD 11)

add_library(trait_runtime
        src/TraitRuntime.c
        src/TraitRuntime.h
        src/TraitRuntime/String/HashStr.c
        src/TraitRuntime/String/HashStr.h
        src/TraitRuntime/Commons/ErrorHandling.h
        src/TraitRuntime/Commons/Callstack.c
        src/TraitRuntime/Commons/Callstack.h
        src/TraitRuntime/Commons/Log.h
        src/TraitRuntime/Commons/Log.c
        src/TraitRuntime/Commons/MacroUtils.h
        src/TraitRuntime/Memory/MemPool.c
        src/TraitRuntime/Memory/MemPool.h
        src/TraitRuntime/Memory/MaxAlign.h
        src/TraitRuntime/Memory/Arena.c
        src/TraitRuntime/Memory/Arena.h
        src/TraitRuntime/Commons/AlertDialog.c
        src/TraitRuntime/Commons/AlertDialog.h
        src/TraitRuntime/Commons/ErrorHandling.c
)

add_executable(demo
        test/main.c
        test/classes/Point.c
        test/classes/Point.h
        test/classes/Vector3f.c
        test/classes/Vector3f.h
        test/traits/Describe.c
        test/traits/Describe.h
        test/traits/Move2i.c
        test/traits/Move2i.h
        test/traits/Move3f.c
        test/traits/Move3f.h
)

target_include_directories(trait_runtime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(demo PRIVATE trait_runtime)

# For TraitRuntime/Commons/Debug.c when on MSVC compiler
if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(trait_runtime PRIVATE Dbghelp)
endif()