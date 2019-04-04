cmake_minimum_required(VERSION 3.13)
project(Oving6)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.54.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenSSL)
include_directories(${OpenSSL_INCLUDE_DIR})

link_libraries(crypto)

add_executable(Oving6 main.cpp)
target_link_libraries(Oving6 ${Boost_LIBRARIES})
target_link_libraries(Oving6 ${OpenSSL_LIBRARIES})