cmake_minimum_required(VERSION 3.31)
project(TraitRuntime C)

set(CMAKE_C_STANDARD 11)

add_library(TraitRuntime
        TraitRuntime/TraitRuntime.c
        TraitRuntime/TraitRuntime.h
        TraitRuntime/string/HashStr.c
        TraitRuntime/string/HashStr.h
        TraitRuntime/common/ErrorHandling.h
        TraitRuntime/common/Debug.c
        TraitRuntime/common/Debug.h
        TraitRuntime/common/Log.h
        TraitRuntime/common/Log.c
        TraitRuntime/common/MacroUtils.h
)

add_executable(AppTest
        AppTest/main.c
        AppTest/types/Point.c
        AppTest/types/Point.h
        AppTest/types/Vector3f.c
        AppTest/types/Vector3f.h
        AppTest/traits/Describe.c
        AppTest/traits/Describe.h
        AppTest/traits/Move2i.c
        AppTest/traits/Move2i.h
        AppTest/traits/Move3f.c
        AppTest/traits/Move3f.h
)

target_include_directories(AppTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/TraitRuntime)
target_link_libraries(AppTest PRIVATE TraitRuntime)

if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(TraitRuntime PRIVATE Dbghelp)
endif()