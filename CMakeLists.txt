cmake_minimum_required(VERSION 3.18)
project(chess_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

# Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)

# Sources and headers
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include directories
include_directories(${INCLUDE_DIR})

# Main executable
set(ENGINE_SOURCES
    src/main.cpp
)

add_executable(chess_engine ${ENGINE_SOURCES})

# GoogleTest
if(BUILD_TESTS)
    enable_testing()
    # FetchContent for GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Test sources and headers
    set(TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

    # Example test executable - we'll add more as we implement modules
    add_executable(test_runner
        tests/test_main.cpp
    )
    target_link_libraries(test_runner PRIVATE gtest_main)
    add_test(NAME test_runner COMMAND test_runner)
endif()

# Benchmarks
if(BUILD_BENCHMARKS)
    # We'll add benchmark executables later
    # For now, just a placeholder
    add_executable(benchmark_perft benchmarks/perft_benchmark.cpp)
    target_link_libraries(benchmark_perft PRIVATE pthread)
endif()

# Install
install(TARGETS chess_engine DESTINATION bin)
if(BUILD_TESTS)
    install(TARGETS test_runner DESTINATION bin)
endif()
if(BUILD_BENCHMARKS)
    install(TARGETS benchmark_perft DESTINATION bin)
endif()
install(DIRECTORY include/ DESTINATION include)