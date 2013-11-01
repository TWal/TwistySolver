cmake_minimum_required(VERSION 2.6)

project(TwistySolver)
SET(CMAKE_CXX_FLAGS "-g -Wall -pedantic -std=c++11")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

file(GLOB_RECURSE source_files src/*)
add_executable(${PROJECT_NAME} ${source_files})
