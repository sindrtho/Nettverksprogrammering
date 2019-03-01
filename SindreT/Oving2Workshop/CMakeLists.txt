cmake_minimum_required(VERSION 3.13)
project(Oving2Workshop)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.54.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(Oving2Workshop echo_server.cpp)
target_link_libraries(Oving2Workshop ${Boost_LIBRARIES})