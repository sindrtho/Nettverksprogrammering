cmake_minimum_required(VERSION 3.10.2)
project(Oving1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Oving1 main.cpp)
add_executable(Oving1Alt altMain.cpp)

find_package(Threads REQUIRED)

target_link_libraries(Oving1 ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Oving1Alt ${CMAKE_THREAD_LIBS_INIT})

target_compile_options(Oving1 PRIVATE -pthread)
target_compile_options(Oving1Alt PRIVATE -pthread)

