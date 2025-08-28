cmake_minimum_required(VERSION 3.15)
project(RRE VERSION 1.0 LANGUAGES C CXX)

# --- C++ Standard ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Source Files ---
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# --- External Dependencies ---
include(FetchContent)

# raylib
find_package(raylib QUIET)
if(NOT raylib_FOUND)
    message(STATUS "raylib not found. Fetching via FetchContent.")
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )
    FetchContent_MakeAvailable(raylib)
endif()

FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG 4.0
)
FetchContent_MakeAvailable(raygui)

# --- Library (DLL) ---
add_library(RRE SHARED ${SRC_FILES})

target_include_directories(RRE PUBLIC "${raygui_SOURCE_DIR}/src")

target_include_directories(RRE
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs>"
    "$<INSTALL_INTERFACE:include>"
)

target_link_libraries(RRE PUBLIC raylib)

# --- Export Symbols for Windows ---
if(WIN32)
    target_compile_definitions(RRE PRIVATE RRE_EXPORTS)
endif()

# --- Examples (optional) ---
option(BUILD_EXAMPLES "Build example programs" OFF)

if(BUILD_EXAMPLES)
    set(EXAMPLE_RESOURCES "${CMAKE_SOURCE_DIR}/examples/resources")
    add_subdirectory(examples)
endif()
