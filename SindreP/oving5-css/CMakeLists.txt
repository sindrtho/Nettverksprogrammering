cmake_minimum_required(VERSION 3.13)
project(oving5_css)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pthread -Wall -Wextra -Wno-unused-parameter")

find_package(odb)
include_directories(${odb_INCLUDE_DIR})

add_executable(oving5_css main.cpp)