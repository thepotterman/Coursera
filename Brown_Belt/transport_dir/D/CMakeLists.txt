cmake_minimum_required(VERSION 3.20)
project(C)

set(CMAKE_CXX_STANDARD 17)

add_executable(C main.cpp Coordinate.h Coordinate.cpp DataBase.h DataBase.cpp BusInfo.h BusInfo.cpp StopInfo.h StopInfo.cpp)
