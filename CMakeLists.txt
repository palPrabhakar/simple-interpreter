cmake_minimum_required(VERSION 3.27)

project(simple_interpreter)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare")

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(test)

