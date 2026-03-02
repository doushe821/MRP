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

CPMAddPackage(
    NAME Eigen
    GITLAB_REPOSITORY libeigen/eigen
    GIT_TAG 5.0.0
    OPTIONS "BUILD_TESTING OFF"
)

CPMAddPackage(
    NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    GIT_TAG v3.5.2
)

if (NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    target_include_directories(Eigen3::Eigen INTERFACE
        ${eigen_SOURCE_DIR}
    )
endif()

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
    Eigen3::Eigen
)


# -----------------------------
# Catch2 test executable
# -----------------------------

add_executable(${TEST_EXEC_NAME}
    tests/AccuracyTests.cpp

    src/EigenValueFinders.cpp
    src/GaussSeidel.cpp
    src/Helpers.cpp
)

set_target_properties(${TEST_EXEC_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)

target_include_directories(${TEST_EXEC_NAME}
    PRIVATE include
    tests/include
)

target_link_libraries(${TEST_EXEC_NAME}
    PRIVATE Catch2::Catch2WithMain
    CLI11::CLI11
    Eigen3::Eigen
)

target_compile_options(${TEST_EXEC_NAME}
    PRIVATE
        ${COMMON_COMPILE_OPTIONS}
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

target_link_options(${TEST_EXEC_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:${DEBUG_COMPILE_OPTIONS}>
        $<$<CONFIG:Release>:${RELEASE_COMPILE_OPTIONS}>
)

# -----------------------------
# Enable testing
# -----------------------------
enable_testing()
add_test(NAME UnitTests COMMAND ${TEST_EXEC_NAME})

# cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++
# cmake --build build
