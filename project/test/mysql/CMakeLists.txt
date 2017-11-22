project(main)
cmake_minimum_required(VERSION 3.0)
#cmake -DCMAKE_BUILD_TYPE=Debug
# g++ -std=c++11 -DSTATIC_CONCPP -I ../../../include/mysql  main.cpp  -L../../../lib/debug/mysql -lmysqlcppconn8 -lpthread -o main
IF (WIN32)  
    MESSAGE(STATUS "Now is windows")  
ELSEIF (APPLE)  
    MESSAGE(STATUS "Now is Apple systens.")  
ELSEIF (UNIX)  
    MESSAGE(STATUS "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")  
ENDIF () 

MESSAGE(STATUS "Linking statically")
add_definitions(-DSTATIC_CONCPP)
# add_definitions(-DWITH_SSL)

include_directories(
    ../../../include
    ../../../include/mysql
)

find_library(pthread_lib pthread /usr/lib /usr/local/lib /usr/lib64 NO_DEFAULT_PATH)
find_library(mysqlcppconn8_lib mysqlcppconn8 ../../../lib/debug/mysql NO_DEFAULT_PATH)

link_libraries(
    ${pthread_lib} 
    ${mysqlcppconn8_lib} 
    )

set(
    SRC_LIST main.cpp
)
add_executable(main ${SRC_LIST})

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(main PRIVATE "-std=c++11")
    MESSAGE(STATUS "change complie to c++11")  
endif()

