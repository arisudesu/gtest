cmake_minimum_required (VERSION 3.5.0)
project(gtest)
find_package(PkgConfig REQUIRED)

set(INCLUDE_DIR ./)
set(SOURCE_DIR ./src)
set(BINARY gtest.bin)

set(SOURCES
    ${SOURCE_DIR}/client.cpp
    ${SOURCE_DIR}/game.cpp
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/shader.cpp
)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS -std=c++11)

pkg_search_module(GLFW REQUIRED glfw3)
find_package(glbinding REQUIRED)

include_directories(${INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLBINDING_INCLUDE_DIR})

add_executable(${BINARY} ${SOURCES})

target_link_libraries(
    ${BINARY}
    ${GLFW_LIBRARIES}
    ${GLBINDING_LIBRARIES}
)
