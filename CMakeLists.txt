cmake_minimum_required(VERSION 3.14)
project(FromScaleProject)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()