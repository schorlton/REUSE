cmake_minimum_required(VERSION 3.5)
project(REUSE)

set(CMAKE_CXX_STANDARD 14)


add_executable(REUSE main.cpp ThreadQueue.cpp ThreadQueue.h cmdline.cpp common.cpp)

