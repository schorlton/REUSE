cmake_minimum_required(VERSION 3.5)
project(REUSE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -pedantic")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

INCLUDE_DIRECTORIES(SYSTEM lib)

add_executable(reuse main.cpp SharedQueue.h cmdline.cpp AbstractKmerContainer.h BBHashKmerContainer.h KmerIterator.h thread_util.cpp thread_util.h KmerIterator.cpp BBHashKmerContainer.cpp filter.cpp build.cpp)

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( reuse ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

find_package( Threads REQUIRED)
if ( Threads_FOUND )
    include_directories( ${Threads_INCLUDE_DIRS} )
    target_link_libraries( reuse ${CMAKE_THREAD_LIBS_INIT} )
endif( Threads_FOUND )
