cmake_minimum_required(VERSION 3.10)
project(klwk LANGUAGES CXX)

add_executable(klwk klwk.cpp)

if(MSVC)
    target_compile_options(klwk PRIVATE /utf-8 /O2)
else()
    target_compile_options(klwk PRIVATE -O2)
endif()
