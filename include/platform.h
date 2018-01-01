#pragma once
#ifndef _PLATFORM_H_
#define _PLATFORM_H_


#if defined (_MSC_VER) 
    #if (_MSC_VER < 1900)
        #error This needs _MSC_VER at least 1900 which suport c++11
    #endif
#elif __cplusplus < 201100L
    #error This library needs at least a C++11 compliant compiler
#endif

#include <thread>
#include <chrono> 

#ifdef _WIN32

#include <windows.h>

#else

#include <unistd.h>

#endif

namespace LU_PLATFORM 
{
    inline void sleep(unsigned long uMilliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(uMilliseconds)); //休眠1毫秒 
    }
};
#endif //_PLATFORM_H_
