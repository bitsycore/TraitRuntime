cmake_minimum_required(VERSION 3.31)
project(src C)

set(CMAKE_C_STANDARD 11)

add_library(TraitRuntime
        src/TraitRuntime.c
        src/TraitRuntime.h
        src/TraitRuntime/String/HashStr.c
        src/TraitRuntime/String/HashStr.h
        src/TraitRuntime/Commons/ErrorHandling.h
        src/TraitRuntime/Commons/Debug.c
        src/TraitRuntime/Commons/Debug.h
        src/TraitRuntime/Commons/Log.h
        src/TraitRuntime/Commons/Log.c
        src/TraitRuntime/Commons/MacroUtils.h
        src/TraitRuntime/Memory/Arena.h
        src/TraitRuntime/Memory/MemPool.c
        src/TraitRuntime/Memory/MemPool.h
)

add_executable(AppTest
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

target_include_directories(TraitRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(AppTest PRIVATE TraitRuntime)

# For TraitRuntime/Commons/Debug.c when on MSVC compiler
if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(TraitRuntime PRIVATE Dbghelp)
endif()