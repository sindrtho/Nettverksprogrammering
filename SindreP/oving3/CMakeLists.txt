cmake_minimum_required(VERSION 3.13)
project(oving3)
set(CMAKE_CXX_STANDARD 14)

set(Boost_LIB_PREFIX            "lib"       CACHE STRING "")
set(Boost_USE_MULTITHREADED     ON          CACHE BOOL "") # '-mt' flag
set(Boost_USE_STATIC_LIBS       ON          CACHE BOOL "")
set(Boost_USE_STATIC_RUNTIME    ON          CACHE BOOL "") # '-s' tag
set(Boost_USE_DEBUG_RUNTIME     ON          CACHE BOOL "") # '-g' tag
set(Boost_COMPILER              "-mgw49"    CACHE STRING "")

set(BOOST_ROOT "C:\\CPP\\Libraries\\boost_1_60_0")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}\\lib64-msvc-14.0")
message(${BOOST_ROOT})

find_package(Boost 1.60.0 REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIR})

add_executable(oving3 echo_server.cpp)
target_link_libraries(oving3 ${Boost_LIBRARIES})