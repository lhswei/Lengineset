#ifndef _LU_TIMER_H_
#define _LU_TIMER_H_

#include "platform.h"

#include <chrono>  

using namespace std;  
using namespace std::chrono;  

template <typename T = chrono::seconds>
class LUClock 
{  
public:  
    LUClock() : m_begin(high_resolution_clock::now()) {}  
    void reset() { m_begin = high_resolution_clock::now(); } 
    //默认输出毫秒  
    int64_t elapsed() const  
    {  
        return duration_cast<chrono::milliseconds>(high_resolution_clock::now() - m_begin).count();  
    }  
    //微秒  
    int64_t elapsed_micro() const  
    {  
        return duration_cast<chrono::microseconds>(high_resolution_clock::now() - m_begin).count();  
    }   
    //纳秒  
    int64_t elapsed_nano() const  
    {  
        return duration_cast<chrono::nanoseconds>(high_resolution_clock::now() - m_begin).count();  
    }  
    //秒  
    int64_t elapsed_seconds() const  
    {  
        return duration_cast<chrono::seconds>(high_resolution_clock::now() - m_begin).count();  
    }  
    //分  
    int64_t elapsed_minutes() const  
    {  
        return duration_cast<chrono::minutes>(high_resolution_clock::now() - m_begin).count();  
    }  
    //时  
    int64_t elapsed_hours() const  
    {  
        return duration_cast<chrono::hours>(high_resolution_clock::now() - m_begin).count();  
    }  

    int64_t elapsed_ticker()
    {
        return duration_cast<T>(high_resolution_clock::now() - m_begin).count();  
    }
private:  
    time_point<high_resolution_clock> m_begin;  
};  

template <typename T = duration<int64_t, ratio<1,1>>>
class LUTimer: public LUClock<T>
{
public:
    LUTimer() : m_pass(high_resolution_clock::now()) {};
    ~LUTimer() {};

    void initialize()
    {
        reset();
        m_pass = high_resolution_clock::now();
        m_ticker = 0;
    }

    int64_t get_pass()
    {
        return duration_cast<T>(high_resolution_clock::now() - m_pass).count();  
    }

    bool ticking()
    {
        int64_t pass = get_pass();
        if(pass > 0)
        {
            m_pass = high_resolution_clock::now();
            m_ticker += pass;
        }


        if(m_ticker < 0)
            return false;

        --m_ticker;
        return true;
    }

private:
    int64_t      m_ticker = 0;
    time_point<high_resolution_clock> m_pass;
};































#endif //_LU_TIMER_H_

