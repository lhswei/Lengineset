IF (WIN32)  
    MESSAGE(STATUS "Now is windows")  

    set(CMAKE_C_COMPILER "gcc")#设置C编译器  
    set(CMAKE_C_FLAGS "-g -Wall  -I e:\\programfile\\mingw-w64\\mingw64\\include -L e:\\programfile\\mingw-w64\\mingw64\\lib")#  
      
    set(CMAKE_CXX_COMPILER "g++")#设置C++编译器  
    set(CMAKE_CXX_FLAGS "-g -Wall  -I e:\\programfile\\mingw-w64\\mingw64\\include -L e:\\programfile\\mingw-w64\\mingw64\\lib") 
ELSEIF (APPLE)  
    MESSAGE(STATUS "Now is Apple systens.")  
ELSEIF (UNIX)  
    MESSAGE(STATUS "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")  
ENDIF () 

project(main)
cmake_minimum_required(VERSION 3.0)
#cmake -G   "MinGW Makefiles" . # .为当前目录, "MinGW Makefiles"为makefile类型，如果编译器为vs的话使用"NMake Makefiles" 
#cmake -DCMAKE_BUILD_TYPE=Debug

IF (WIN32)  
    add_definitions(-D_WIN32)
ELSEIF (APPLE)  

ELSEIF (UNIX)  

ENDIF () 


include_directories(
    ../../../include
    ../../app
)

set(
    SRC_LIST main.cpp
)

aux_source_directory(../../app APP_SRC_LIST)
list(APPEND SRC_LIST ${APP_SRC_LIST})


add_executable(main ${SRC_LIST})

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(main PRIVATE "-std=c++11")
    MESSAGE(STATUS "change complie to c++11")  
endif()


