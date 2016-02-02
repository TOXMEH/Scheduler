cmake_minimum_required(VERSION 3.2.2)
project(Scheduler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
        tasks.txt)
add_executable(Scheduler ${SOURCE_FILES})