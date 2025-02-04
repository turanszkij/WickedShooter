cmake_minimum_required(VERSION 3.8)
project(WickedShooter)

add_subdirectory(WickedEngine)

set(SOURCE_FILES
    main_linux.cpp
    stdafx.h
)

set(PLATFORM "SDL2")
add_compile_definitions(SDL2=1)

set(LIB_DXCOMPILER "libdxcompiler.so")

add_executable(WickedShooter ${SOURCE_FILES})

target_include_directories(WickedShooter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/WickedEngine/WickedEngine)

target_link_libraries(WickedShooter PUBLIC WickedEngine)

