cmake_minimum_required(VERSION 3.5)
project(REUSE)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(lib)

add_executable(reuse main.cpp SharedQueue.cpp SharedQueue.h cmdline.cpp common.cpp)

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( reuse ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )