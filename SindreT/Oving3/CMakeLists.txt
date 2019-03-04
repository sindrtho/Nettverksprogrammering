cmake_minimum_required(VERSION 3.13)
project(Oving3)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pthread -Wall -Wextra -Wno-unused-parameter")

find_package(Boost 1.54.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(Server Server.cpp kalk.h Kalkulator.cpp)
target_link_libraries(Server ${Boost_LIBRARIES})
add_executable(Klient Client.cpp kalk.h Kalkulator.cpp)
target_link_libraries(Klient ${Boost_LIBRARIES})
add_executable(Test oleSinKode.cpp)
target_link_libraries(Test ${Boost_LIBRARIES})