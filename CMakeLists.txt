# Copyright (c) 2019 - Herman Polloni.
# This code is licensed under MIT license (see LICENSE for details)

cmake_minimum_required (VERSION 2.8.11)

project(netifacescpp)

set(CMAKE_CXX_STANDARD 14)

find_program(clang_tidy "clang-tidy")
if(clang_tidy)
    message("Clang-tidy found in ${clang_tidy}")
    set(CMAKE_CXX_CLANG_TIDY ${clang_tidy})
endif()

add_library(netifacescpp INTERFACE)
target_include_directories(netifacescpp INTERFACE include/)

add_executable(list_all examples/list_all.cc)
target_link_libraries(list_all netifacescpp)
