cmake_minimum_required(VERSION 3.13)
project(oving3)
set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.60.0 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})

add_executable(oving3 echo_server.cpp)
target_link_libraries(oving3 ${Boost_LIBRARIES})