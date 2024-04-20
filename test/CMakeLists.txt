enable_testing()

find_package(GTest REQUIRED)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/test/*.cpp)

add_executable(test ${SOURCE_FILES})

target_link_libraries(test PRIVATE sci GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(test)
