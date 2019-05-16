cmake_minimum_required(VERSION 2.8.7)
project(test_nanostl)

# cmake modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/sanitizers)

set(CMAKE_CXX_STANDARD 11)

add_executable(test_nanostl test.cc test_valarray.cc)

target_include_directories(test_nanostl PRIVATE "../include")
