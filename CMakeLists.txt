cmake_minimum_required(VERSION 3.14)
project(HtmlParser)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_library(HtmlParser STATIC src/Parser.cpp)

add_subdirectory(tests)