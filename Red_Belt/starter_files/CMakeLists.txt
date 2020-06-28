cmake_minimum_required(VERSION 3.16)
project(starter_files)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(starter_files
        iterator_range.h
        main.cpp
        parse.cpp
        parse.h
        search_server.cpp
        search_server.h)
