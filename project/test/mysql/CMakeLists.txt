project(main)
cmake_minimum_required(VERSION 3.0)
#cmake -DCMAKE_BUILD_TYPE=Debug

include_directories(
    ../../../include
    ../../../include/mysql/linux
)

find_library(mysqlclient_lib mysqlclient ../../../lib/debug/mysql NO_DEFAULT_PATH)
find_library(pthread_lib pthread /usr/lib /usr/local/lib /usr/lib64 NO_DEFAULT_PATH)
find_library(dl_lib dl /usr/lib /usr/local/lib /usr/lib64 NO_DEFAULT_PATH)

link_libraries(
    ${mysqlclient_lib} 
    ${pthread_lib} 
    ${dl_lib} 
    )

set(
    SRC_LIST main.cpp
    mydb.cpp
)
add_executable(main ${SRC_LIST})