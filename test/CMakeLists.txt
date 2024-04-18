enable_testing()

ind_package(GTest REQUIRED)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/test/*.cpp)

add_executable(test ${SOURCE_FILES})

target_link_libraries(test PRIVATE tci GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(test)
