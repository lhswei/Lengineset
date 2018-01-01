project(main)
cmake_minimum_required(VERSION 3.0)
#cmake -DCMAKE_BUILD_TYPE=Debug
#cmake -G   "MinGW Makefiles" . # .为当前目录, "MinGW Makefiles"为makefile类型，如果编译器为vs的话使用"NMake Makefiles" 

IF (WIN32)  
    MESSAGE(STATUS "Now is windows")  
    IF (NOT CMAKE_C_COMPILER)
        MESSAGE(STATUS "Now set gcc mingw-w64") 
        set(CMAKE_C_COMPILER "gcc")#设置C编译器  
        set(CMAKE_C_FLAGS "-g -Wall  -I e:\\programfile\\mingw-w64\\mingw64\\include -L e:\\programfile\\mingw-w64\\mingw64\\lib")
    ENDIF ()

    IF (NOT CMAKE_CXX_COMPILER)
        MESSAGE(STATUS "Now set g++ mingw-w64") 
        set(CMAKE_CXX_COMPILER "g++")#设置C++编译器  
        set(CMAKE_CXX_FLAGS "-g -Wall  -I e:\\programfile\\mingw-w64\\mingw64\\include -L e:\\programfile\\mingw-w64\\mingw64\\lib") 
    ENDIF ()
ELSEIF (APPLE)  
    MESSAGE(STATUS "Now is Apple systens.")  
ELSEIF (UNIX)  
    MESSAGE(STATUS "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")  
ENDIF () 

include_directories(
    ../../../../include
)

set(
    SRC_LIST main.cpp
)
add_executable(main ${SRC_LIST})

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(main PRIVATE "-std=c++11")
    MESSAGE(STATUS "change complie to c++11")  
endif()
