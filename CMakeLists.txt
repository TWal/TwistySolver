cmake_minimum_required(VERSION 2.6)

project(TwistySolver)
SET(CMAKE_CXX_FLAGS "-g -Wall -pedantic -std=c++11")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

set(${PROJECT_NAME}_SOURCES
    src/Cube.cpp
    src/Coordinate.cpp
    src/OrientationCoordinate.cpp
    src/PermutationCoordinate.cpp
    src/PositionCoordinate.cpp
    src/Solver333.cpp
    src/Utils.cpp
)

add_library(
    ${PROJECT_NAME}_lib
    SHARED
    ${${PROJECT_NAME}_SOURCES}
)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    ${PROJECT_NAME}_lib
)

enable_testing()

set(${PROJECT_NAME}_TESTS
    tests/cube.cpp
    tests/coordinate.cpp
    tests/utils.cpp
    tests/solver.cpp
)

include_directories(src)

add_executable(
    ${PROJECT_NAME}Test
    ${${PROJECT_NAME}_TESTS}
)

target_link_libraries(${PROJECT_NAME}Test
    ${PROJECT_NAME}_lib
    gtest
    gtest_main
)

add_test(
    NAME ${PROJECT_NAME}Test
    COMMAND ${PROJECT_NAME}Test
)

