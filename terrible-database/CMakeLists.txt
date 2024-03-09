cmake_minimum_required(VERSION 3.13)

project(terrible_database)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(test)
endif()

