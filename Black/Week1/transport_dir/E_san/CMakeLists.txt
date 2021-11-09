cmake_minimum_required(VERSION 3.20)
project(E_san)

set(CMAKE_CXX_STANDARD 17)

add_executable(E_san main.cpp descriptions.cpp descriptions.h graph.h json.h json.cpp requests.cpp
        requests.h router.h sphere.h sphere.cpp transport_catalog.h transport_catalog.cpp
        transport_router.h transport_router.cpp utils.h utils.cpp)
