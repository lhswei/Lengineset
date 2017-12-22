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

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(-DSPDLOG_TRACE_ON)
add_definitions(-DSPDLOG_DEBUG_ON)

include_directories(
    ../../../include
)

find_package(Threads)

set(
    SRC_LIST main.cpp
)
add_executable(main ${SRC_LIST})
target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(main PRIVATE "-std=c++11")
    MESSAGE(STATUS "change complie to c++11")  
endif()

