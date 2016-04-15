cmake_minimum_required (VERSION 3.5.0)
project(gtest)
find_package(PkgConfig REQUIRED)

set(INCLUDE_DIR ./)
set(SOURCE_DIR ./src)
set(BINARY gtest.bin)

set(SOURCES
    ${SOURCE_DIR}/client.cpp
    ${SOURCE_DIR}/game.cpp
    ${SOURCE_DIR}/guimenu.cpp
    ${SOURCE_DIR}/shader.cpp
    ${SOURCE_DIR}/textrenderer.cpp
    ${SOURCE_DIR}/gameplay.cpp
    ${SOURCE_DIR}/main.cpp
)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-g -march=native -std=c++11 -pedantic -Wall -Wextra -Weffc++ -fno-elide-constructors")

pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(FT2 REQUIRED freetype2)
pkg_search_module(OPENAL REQUIRED openal)
find_package(glbinding REQUIRED)

include_directories(${INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${FT2_INCLUDE_DIRS})
include_directories(${GLBINDING_INCLUDE_DIR})
include_directories(${OPENAL_INCLUDE_DIR})

add_executable(${BINARY} ${SOURCES})

target_link_libraries(
    ${BINARY}
    ${GLFW_LIBRARIES}
    ${FT2_LIBRARIES}
    ${GLBINDING_LIBRARIES}
    ${OPENAL_LIBRARIES}
)
