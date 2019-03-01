cmake_minimum_required(VERSION 3.13)
project(Oving1)

set(CMAKE_CXX_STANDARD 14)

add_executable(testt RunTest.cpp )
add_test(testt testt)
enable_testing()