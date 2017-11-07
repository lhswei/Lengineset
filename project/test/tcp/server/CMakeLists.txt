project(server)
cmake_minimum_required(VERSION 3.0)
#cmake -DCMAKE_BUILD_TYPE=Debug

include_directories(
    ../../../../include
)

set(
    SRC_LIST tcp_servertest.cpp
)
add_executable(server ${SRC_LIST})