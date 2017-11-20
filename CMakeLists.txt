project(hello)
cmake_minimum_required(VERSION 3.0)

IF (WIN32)  
    MESSAGE(STATUS "Now is windows")  
ELSEIF (APPLE)  
    MESSAGE(STATUS "Now is Apple systens.")  
ELSEIF (UNIX)  
    MESSAGE(STATUS "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")  
ENDIF () 

aux_source_directory(. DIR_SRCS)
add_executable(hello ${DIR_SRCS})