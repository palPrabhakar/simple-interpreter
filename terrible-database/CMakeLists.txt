cmake_minimum_required(VERSION 3.13)

project(terrible-database)

set(CMAKE_CXX_STANDARD 20)

# Add main.cpp file of the project root directory as a source file
file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/includes/*.h)
# set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/parser.cpp)
# set(HEADER_FILES ${CMAKE_SOURCE_DIR}/includes/tokenizer.h)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(tdb ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(tdb PRIVATE ${CMAKE_SOURCE_DIR}/includes)
