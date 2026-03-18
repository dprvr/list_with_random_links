cmake_minimum_required(VERSION 3.14)
project(ListWithRandomLinks)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build tests" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)