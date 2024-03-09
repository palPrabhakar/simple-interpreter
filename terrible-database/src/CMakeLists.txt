file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/includes/*.h)

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(tdb ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(tdb PUBLIC ${CMAKE_SOURCE_DIR}/includes)
