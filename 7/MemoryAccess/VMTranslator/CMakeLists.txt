cmake_minimum_required(VERSION 3.16)

project(VMTranslator LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to Debug build if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Strict compile flags (similar to your Makefile)
add_compile_options(
    -ggdb
    -g3
    -g
    -O0
    -pedantic-errors
    -Wall
    -Wextra
    -Wconversion
    -Wsign-conversion
    -Werror
)

# Include headers
include_directories(inc)

# Collect source files
file(GLOB SRC_FILES src/*.cpp)

# Create executable
add_executable(vmtranslator ${SRC_FILES})
