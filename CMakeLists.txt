cmake_minimum_required (VERSION 3.20)
project (MRP CXX)

# -----------------------------
# General settings
# -----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(TEST_EXEC_NAME "${PROJECT_NAME}Tests")

# -----------------------------
# CPM Setup
# -----------------------------
include(cmake/CPM.cmake)

# -----------------------------
# Dependencies via CPM
# -----------------------------

CPMAddPackage(
    NAME CLI11
    GITHUB_REPOSITORY CLIUtils/CLI11
    GIT_TAG v2.3.2
)

# -----------------------------
# Compiler options
# -----------------------------
set(COMMON_COMPILE_OPTIONS
    -fdiagnostics-color=always
    -Wall
    -Wextra
    -fstack-protector-strong
    -fcheck-new
    -fstrict-overflow
    -march=native
)

set(DEBUG_COMPILE_OPTIONS
    -Og
    -g3
    -DDEBUG
    -ggdb
    -fsanitize=address,leak,undefined
)

set(RELEASE_COMPILE_OPTIONS
    -O2
    -DNDEBUG
)

# -----------------------------
# Compiler executable
# -----------------------------

set(LOG_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/log
)

file(MAKE_DIRECTORY ${LOG_DIR})

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/GaussSeidel.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        ${COMMON_COMPILE_OPTIONS}
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_link_options(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    CLI11::CLI11
)

# cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++
# cmake --build build