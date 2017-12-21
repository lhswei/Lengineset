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

# include_directories(
#     /usr/local/include
# )

# find_library(boost_thread_lib boost_thread /usr/local/lib NO_DEFAULT_PATH)
# find_library(boost_system_lib boost_system /usr/local/lib NO_DEFAULT_PATH)

# link_libraries(
#     ${boost_thread_lib} 
#     ${boost_system_lib} 
#     )

set(
    SRC_LIST main.cpp
)
add_executable(main ${SRC_LIST})