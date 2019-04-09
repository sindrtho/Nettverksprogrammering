cmake_minimum_required(VERSION 3.13)
project(Oving7)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pthread -Wall -Wextra -Wno-unused-parameter")

if (GCRYPT_LIBRARIES AND GCRYPT_INCLUDE_DIRS)
    # in cache already
    # set(GCRYPT_FOUND TRUE)
else (GCRYPT_LIBRARIES AND GCRYPT_INCLUDE_DIRS)

    set(_GCRYPT_ROOT_PATHS
            "$ENV{PROGRAMFILES}/libgcrypt"
            )

    find_path(GCRYPT_ROOT_DIR
            NAMES
            include/gcrypt.h
            PATHS
            ${_GCRYPT_ROOT_PATHS}
            )
    mark_as_advanced(ZLIB_ROOT_DIR)

    find_path(GCRYPT_INCLUDE_DIR
            NAMES
            gcrypt.h
            PATHS
            /usr/local/include
            /opt/local/include
            /sw/include
            /usr/lib/sfw/include
            ${GCRYPT_ROOT_DIR}/include
            )
    set(GCRYPT_INCLUDE_DIRS ${GCRYPT_INCLUDE_DIR})

    find_library(GCRYPT_LIBRARY
            NAMES
            gcrypt
            gcrypt11
            libgcrypt-11
            PATHS
            /opt/local/lib
            /sw/lib
            /usr/sfw/lib/64
            /usr/sfw/lib
            ${GCRYPT_ROOT_DIR}/lib
            )
    set(GCRYPT_LIBRARIES ${GCRYPT_LIBRARY})

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(GCrypt DEFAULT_MSG GCRYPT_LIBRARIES GCRYPT_INCLUDE_DIRS)

    # show the GCRYPT_INCLUDE_DIRS and GCRYPT_LIBRARIES variables only in the advanced view
    mark_as_advanced(GCRYPT_INCLUDE_DIRS GCRYPT_LIBRARIES)

endif (GCRYPT_LIBRARIES AND GCRYPT_INCLUDE_DIRS)

find_package(Boost 1.54.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(Oving7 main.cpp)
target_link_libraries(Oving7 ${Boost_LIBRARIES})
target_link_libraries(Oving7 ${GCRYPT_LIBRARIES})