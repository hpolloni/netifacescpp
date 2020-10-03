cmake_minimum_required (VERSION 2.8.11)

project(netifacescpp)

set(CMAKE_CXX_STANDARD 14)

add_library(netifacescpp INTERFACE)
target_include_directories(netifacescpp INTERFACE include/)

add_executable(list_all tools/list_all.cc)
target_link_libraries(list_all netifacescpp)
