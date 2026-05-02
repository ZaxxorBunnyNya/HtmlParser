cmake_minimum_required(VERSION 3.31.6)
project(parser_tests)

include(FetchContent)

# Automatically download/build GTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

# Build test executable and link GTest
add_executable(parser_tests BasicTests.cpp)
target_link_libraries(parser_tests GTest::gtest_main HtmlParser)
link_libraries(parser_tests HtmlParser)

# Enable testing and discovery
include(GoogleTest)
gtest_discover_tests(parser_tests)