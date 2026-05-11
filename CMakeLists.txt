cmake_minimum_required(VERSION 3.14)
project(HtmlParser)

set(CMAKE_CXX_STANDARD 20)

include(CheckCXXCompilerFlag)

include_directories(include)
add_library(HtmlParser STATIC src/Parser.cpp src/Tokenizer.cpp)

check_cxx_compiler_flag("-msse4.2" COMPILER_SUPPORTS_SSE42)
if(COMPILER_SUPPORTS_SSE42)
    target_compile_options(HtmlParser PRIVATE -msse4.2)
    target_compile_definitions(HtmlParser PRIVATE HAVE_SSE42)
endif()

add_subdirectory(tests)