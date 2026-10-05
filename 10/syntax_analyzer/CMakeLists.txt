cmake_minimum_required(VERSION 3.16)

project(SyntaxAnalyzer
    VERSION 0.1.0
    LANGUAGES CXX
)

# ---------------------------------------------------------
# Global Settings & Tooling Support
# ---------------------------------------------------------

# Export compile_commands.json for clangd / editor LSPs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type to Debug if none provided
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Place compiled binaries into <build_dir>/bin for tidy organization
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---------------------------------------------------------
# Target Definition
# ---------------------------------------------------------

add_executable(syntaxanalyzer
    src/main.cpp
    src/JackTokenizer.cpp
    # Add other .cpp files here as you create them:
    # src/parser.cpp
    # src/code_writer.cpp
)

# C++ Standard requirements (target-scoped)
target_compile_features(syntaxanalyzer PRIVATE cxx_std_17)
set_target_properties(syntaxanalyzer PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Header include paths
target_include_directories(syntaxanalyzer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# ---------------------------------------------------------
# Compiler Warnings & Diagnostics
# ---------------------------------------------------------

# Compiler-specific warnings (avoids breaking if compiled on MSVC)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(syntaxanalyzer PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -pedantic-errors
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Werror
    )
elseif(MSVC)
    target_compile_options(syntaxanalyzer PRIVATE
        /W4
        /WX
        /permissive-
    )
endif()

# ---------------------------------------------------------
# Optional: Sanitizers for Debugging (ASan & UBSan)
# ---------------------------------------------------------

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" ON)

if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(syntaxanalyzer PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(syntaxanalyzer PRIVATE -fsanitize=address,undefined)
endif()
