# cmake requirements
cmake_minimum_required(VERSION 3.14...4.0)

project(
    8-Puzzle-Game
    VERSION 0.1
    DESCRIPTION "8 Puzzle Game"
    LANGUAGES C CXX
)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# check if we are in the main CMakeLists
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # ensure -std=c++xx instead of -std=g++xx for protability purposes
    set(CMAKE_CXX_EXTENSIONS OFF)

    # use CTest
    # NOTE: this needs to be done in the main CMakeLists
    include(CTest)

    # Docs only available in the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found")
    endif()
endif()

# FetchContent helps grabbing repos during the configure step
# NOTE: available after CMake 3.11
include(FetchContent)

# all required packages
# Try to find the fmt library that is at least newer than 11.2.0 on the machine
find_package(fmt CONFIG 11.0.0 QUIET)
if (NOT fmt_FOUND)
    message(STATUS "fmt could not be found or version is too old, downloading via FetchContent...")
    # all required packages (will be downloaded automatically)
    FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        11.2.0)
    FetchContent_MakeAvailable(fmt)
endif()

function(apply_compiler_flags target_name)
    # compiler flags
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Test" OR CMAKE_BUILD_TYPE STREQUAL "Benchmark")
        add_compile_options(-Wall -Wextra -pedantic -Wconversion -Wsign-conversion -O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-Wall -Wextra -pedantic -g3 -fsanitize=address,undefined)
    endif()

    # linker flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_link_options(-g3 -fsanitize=address,undefined)
    endif()
endfunction()

# the compiled library code
add_subdirectory(src)

# the executable code
add_subdirectory(apps)

# checks if we are in the main CMakeLists
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING AND CMAKE_BUILD_TYPE STREQUAL "Test")
    add_subdirectory(tests)
endif()
