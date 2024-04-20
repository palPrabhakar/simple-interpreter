file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(sci ${SOURCE_FILES})

target_include_directories(sci PUBLIC ${CMAKE_SOURCE_DIR}/includes)

target_link_libraries(sci)
