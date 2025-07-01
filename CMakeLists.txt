cmake_minimum_required(VERSION 3.31)
project(TraitRuntime C)

set(CMAKE_C_STANDARD 11)

add_executable(TraitRuntime
        main.c
        TraitRuntime.c
        TraitRuntime.h
        string/HashStr.c
        string/HashStr.h
        types/Point.c
        types/Point.h
        types/Vector3f.c
        types/Vector3f.h
        traits/Format.c
        traits/Format.h
        traits/Move2i.c
        traits/Move2i.h
        traits/Move3f.c
        traits/Move3f.h
        common/ErrorHandling.h
        common/Debug.c
        common/Debug.h
        common/Log.h
        common/Log.c
        common/MacroUtils.h
)

if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(TraitRuntime PRIVATE Dbghelp)
endif()