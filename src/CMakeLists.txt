file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(tci ${SOURCE_FILES})

target_include_directories(tci PUBLIC ${CMAKE_SOURCE_DIR}/includes)

target_link_libraries(tci)
