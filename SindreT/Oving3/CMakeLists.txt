cmake_minimum_required(VERSION 3.13)
project(Oving3)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.54.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(Server Server.cpp kalk.h Kalkulator.cpp)
target_link_libraries(Server ${Boost_LIBRARIES})
add_executable(Klient Server.cpp kalk.h Kalkulator.cpp Client.cpp)
target_link_libraries(Klient ${Boost_LIBRARIES})