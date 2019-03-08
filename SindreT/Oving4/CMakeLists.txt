cmake_minimum_required(VERSION 3.13)
project(Oving4)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pthread -Wall -Wextra -Wno-unused-parameter")

find_package(Boost 1.54.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(Server EchoServer.cpp)
target_link_libraries(Server ${Boost_LIBRARIES})
add_executable(Client EchoClient.cpp)
target_link_libraries(Client ${Boost_LIBRARIES})
add_executable(Test test.cpp)
target_link_libraries(Test ${Boost_LIBRARIES})