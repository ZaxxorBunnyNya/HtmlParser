cmake_minimum_required(VERSION 3.14)
project(parser_tests)
enable_testing()

include(FetchContent)

# Automatically download/build GTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Build test executable and link GTest
add_executable(parser_tests BasicTests.cpp TokenizerTests.cpp)
target_link_libraries(parser_tests GTest::gtest_main HtmlParser)
link_libraries(parser_tests HtmlParser)

# Enable testing and discovery
include(GoogleTest)
gtest_discover_tests(parser_tests)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/test_data"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
