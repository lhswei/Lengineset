project(main)
cmake_minimum_required(VERSION 3.0)
#cmake -DCMAKE_BUILD_TYPE=Debug

IF (WIN32)  
    MESSAGE(STATUS "Now is windows")  
ELSEIF (APPLE)  
    MESSAGE(STATUS "Now is Apple systens.")  
ELSEIF (UNIX)  
    MESSAGE(STATUS "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")  
ENDIF () 

include_directories(
    ../../../../include
)

set(
    SRC_LIST tcp_servertest.cpp
)
add_executable(main ${SRC_LIST})