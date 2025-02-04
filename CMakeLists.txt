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

find_package(SDL2 REQUIRED)

set(SOURCE_FILES
    main_linux.cpp
    stdafx.h
)

set(LIB_DXCOMPILER "libdxcompiler.so")

add_executable(WickedShooter ${SOURCE_FILES})

target_include_directories(WickedShooter PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/WickedEngine/WickedEngine
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(WickedShooter PUBLIC 
    WickedEngine
    ${SDL2_LIBRARIES}
)

