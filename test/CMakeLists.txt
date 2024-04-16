enable_testing()

find_package(GTest REQUIRED)

add_executable(test ifelse_test.cpp expr_test.cpp)

target_link_libraries(test PRIVATE tci GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(test)
