project(main)
cmake_minimum_required(VERSION 3.0)

IF (WIN32)  
    MESSAGE(STATUS "Now is windows")  

    IF (${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
        IF(NOT DEFINED ENV{MINGW_W64})
            # 没有找到MINGW_W64环境变量
            MESSAGE(STATUS "have not defined environment variable:MINGW_W64")
            # 如果环境变量没有配，请在这里配置
            SET(ENV{MINGW_W64} "e:\\programfile\\mingw-w64\\mingw64")
        ENDIF()
            
        SET(MINGW_W64_INCLUDE "$ENV{MINGW_W64}\\include")
        MESSAGE(STATUS "MINGW_W64_INCLUDE: ${MINGW_W64_INCLUDE}")
        SET(MINGW_W64_LIB "$ENV{MINGW_W64}\\lib")
        MESSAGE(STATUS "MINGW_W64_LIB: ${MINGW_W64_LIB}")
        IF (NOT CMAKE_C_COMPILER)
            MESSAGE(STATUS "Now set gcc mingw-w64") 
            SET(CMAKE_C_COMPILER "gcc")#设置C编译器  
            SET(CMAKE_C_FLAGS "-g -Wall  -I ${MINGW_W64_INCLUDE} -L ${MINGW_W64_LIB}")
        ENDIF ()
        
        IF (NOT CMAKE_CXX_COMPILER)
            MESSAGE(STATUS "Now set g++ mingw-w64") 
            SET(CMAKE_CXX_COMPILER "g++")#设置C++编译器  
            SET(CMAKE_CXX_FLAGS "-g -Wall  -I ${MINGW_W64_INCLUDE} -L ${MINGW_W64_LIB}") 
        ENDIF ()

        SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb ")
        # SET(CMAKE_CXX_FLAGS_RELEASE "${ENV{CXXFLAGS} -O3 -Wall")
    ELSEIF (${CMAKE_GENERATOR} STREQUAL "NMake Makefiles")
        # SET(MY_VCVARSALL_BAT "c:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat")
        # SET(CMAKE_BINARY_DIR, "F:/github/Lengineset/project/test/app/wnd_build")
        # SET(CMAKE_CACHEFILE_DIR, "F:/github/Lengineset/project/test/app/wnd_build")
    ENDIF ()

    add_definitions(-D_WIN32)
    MESSAGE(STATUS ${CMAKE_GENERATOR})
ELSEIF (APPLE)  
    MESSAGE(STATUS "Now is Apple systens.")  
ELSEIF (UNIX)  
    MESSAGE(STATUS "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")  
ENDIF () 

#cmake -G   "MinGW Makefiles" . # .为当前目录, "MinGW Makefiles"为makefile类型，如果编译器为vs的话使用"NMake Makefiles" 
# 建议先建立 mingw_build 到 mingw_build 目录里面用 cmake -DCMAKE_BUILD_TYPE=Debug -G   "MinGW Makefiles" ../
# mingw32-make.exe 如果没有make.exe
#cmake -G   "NMake Makefiles" . # 先到vcvars64.bat所在的目录用CMD命令 运行 vcvars64.bat， 然后切换会 CMakeLists.txt 所在的目录执行 nmake
# 建议先建立 wnd_build 到 wnd_build 目录里面用 cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=./cmake_install -G "NMake Makefiles" ../

#cmake -DCMAKE_BUILD_TYPE=Debug
SET(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")

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


