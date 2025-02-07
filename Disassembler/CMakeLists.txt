cmake_minimum_required(VERSION 3.10)

# Project name
project(rv32dis)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Directories
set(HEADERS_DIR ${CMAKE_SOURCE_DIR}/headers)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Include directories
include_directories(${HEADERS_DIR})


FILE(GLOB SOURCES ./src/*.cpp)

FILE(GLOB HEADERS ./headers/*.h)

add_executable(
    rv32dis
    ${SOURCES}
    ${HEADERS}
)
