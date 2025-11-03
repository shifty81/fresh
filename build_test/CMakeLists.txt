cmake_minimum_required(VERSION 3.20)
project(FreshVoxelEngineTests VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
enable_testing()

# Find Google Test
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "Google Test not found, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Test sources - only new tests
set(TEST_SOURCES
    ../tests/scripting/EventSystemTests.cpp
    ../tests/generation/NoiseTests.cpp
)

# Engine sources needed
set(ENGINE_SOURCES
    ../src/generation/NoiseGenerator.cpp
    ../src/scripting/EventSystem.cpp
)

# Create test executable
add_executable(FreshVoxelEngineTests ${TEST_SOURCES} ${ENGINE_SOURCES})

# Include directories
target_include_directories(FreshVoxelEngineTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Link with Google Test
target_link_libraries(FreshVoxelEngineTests PRIVATE
    GTest::gtest
    GTest::gtest_main
)

# Compiler settings
target_compile_features(FreshVoxelEngineTests PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(FreshVoxelEngineTests PRIVATE /W4)
else()
    target_compile_options(FreshVoxelEngineTests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Register tests
include(GoogleTest)
gtest_discover_tests(FreshVoxelEngineTests)
