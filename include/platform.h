#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#if __cplusplus < 201100L
  #error This library needs at least a C++11 compliant compiler
#endif

#ifdef __WIN32

#include <windows.h>

#else

#include <unistd.h>

#endif

namespace LU_PLATFORM 
{
    // 单位：毫秒
    void sleep(unsigned uMilliseconds)
    {
#ifdef _WIN32
        ::Sleep(uMilliseconds);
#else
        usleep(uMilliseconds * MILLIS_PER_SECONDE);
#endif
    }


};


#endif //_PLATFORM_H_
