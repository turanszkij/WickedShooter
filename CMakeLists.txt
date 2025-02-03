cmake_minimum_required(VERSION 3.8)
project(WickedShooter)

# if the engine is installed system-wise,
# you can set INSTALLED_ENGINE to ON
# moving your project folder wherever you want
set(INSTALLED_ENGINE OFF)
# you may also want to remove the copy mechanism of the startup.lua file

if (${INSTALLED_ENGINE})
    find_package(WickedEngine REQUIRED)
endif()

set(SOURCE_FILES
    main_linux.cpp
    stdafx.h
)

set(LIB_DXCOMPILER "libdxcompiler.so")

add_executable(WickedShooter ${SOURCE_FILES})

target_link_libraries(WickedShooter PUBLIC
    # These are cmake generator expressions. They seem more daunting than they are
    # Basically if the variable is ON it will give WickedEngine,
    # if it's off, it will give WickedEngine::WickedEngine
    $<$<NOT:$<BOOL:${INSTALLED_ENGINE}>>:WickedEngine>
    $<$<BOOL:${INSTALLED_ENGINE}>:WickedEngine::WickedEngine>
)

