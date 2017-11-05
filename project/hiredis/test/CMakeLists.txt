project(hiredis_test)
cmake_minimum_required(VERSION 3.0)
#cmake -DCMAKE_BUILD_TYPE=Debug
#set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug") 

include_directories(
    ../../../include
    ../../../include/libevent
)

find_library(runtime_lib rt /usr/lib /usr/local/lib /usr/lib64 NO_DEFAULT_PATH)
find_library(hiredis_lib hiredis ../../../lib/debug NO_DEFAULT_PATH)
find_library(event_lib event ../../../lib/debug NO_DEFAULT_PATH)
find_library(event_core_lib event_core ../../../lib/debug NO_DEFAULT_PATH)
find_library(event_extra_lib event_extra ../../../lib/debug NO_DEFAULT_PATH)

link_libraries(
    ${runtime_lib} 
    ${hiredis_lib} 
    ${event_lib} 
    ${event_core_lib}
    ${event_extra_lib}
    )

set(
    SRC_LIST main.cpp
    ../redisclient.cpp
    ../redisasyncclient.cpp
)
add_executable(hiredis_test ${SRC_LIST})