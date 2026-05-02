cmake_minimum_required(VERSION 3.31.6)
project(HtmlParser)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
add_library(HtmlParser STATIC src/Parser.cpp)

add_subdirectory(tests)