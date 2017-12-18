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

# add_definitions(-DLUA_BUILD_AS_DLL)

include_directories(
    ../../../include
    ../../../include/lua
    ../../../submodule/lua/src
)

find_library(lua_lib lua ../../../lib/debug NO_DEFAULT_PATH)


link_libraries(
    ${lua_lib} 
    )

set(
    SRC_LIST luamain.cpp
)
add_executable(main ${SRC_LIST})