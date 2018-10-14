cmake_minimum_required(VERSION 3.5)
project(REUSE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH "lib/seqan/util/cmake")
set(SEQAN_INCLUDE_PATH "lib/seqan/include")


#set(SEQAN_INCLUDE_PATH lib/seqan)

INCLUDE_DIRECTORIES(lib)

add_executable(reuse main.cpp SharedQueue.h cmdline.cpp common.cpp thread_util.cpp thread_util.h)

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( reuse ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

# Load the SeqAn module and fail if not found.
#find_package (SeqAn REQUIRED)

# Add include directories.
#include_directories (${SEQAN_INCLUDE_DIRS})

# Add definitions set by find_package (SeqAn).
#add_definitions (${SEQAN_DEFINITIONS})

# Add CXX flags found by find_package (SeqAn).
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SEQAN_CXX_FLAGS}")

# Add executable and link against SeqAn dependencies.

#target_link_libraries (reuse ${SEQAN_LIBRARIES})