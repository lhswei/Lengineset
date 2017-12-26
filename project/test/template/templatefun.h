
#ifndef _TEMPLATE_FUN_H_
#define _TEMPLATE_FUN_H_

#include <stdarg.h>
#include <string>
#include "templatedef.h"
#include <memory>
#include <vector>

using namespace std;

template <typename T>
std::string tf_number2str(T a)
{
    int t = static_cast<int>(a);
    char ch[16];
    memset(ch, 0, 16);
    sprintf(ch, "%d", t);
    return std::string(ch);
}

template <>
std::string tf_number2str(float a)
{
    char ch[16];
    memset(ch, 0, 16);
    sprintf(ch, "%0.2f", a); 
    return std::string(ch);
}

template <>
std::string tf_number2str(double a)
{
    char ch[16];
    memset(ch, 0, 16);
    sprintf(ch, "%0.4f", a); 
    return std::string(ch);
}


template <typename T>
T tf_add(const T& a, const T& b)
{
    return a + b;
}


template<class T>
void tf_h(T a, T b){}


template<typename T>
const T& tf_max(const T& a,const T& b){
    return a>b ? a:b;
}

class LConsole 
{
public:
    LConsole() {}
    ~LConsole() {}
    inline int info(const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        printf(format, ap);
        va_end(ap);
        printf("\n");
        return 1;
    }

};

template <typename T>
inline int tf_defaultparam(std::string fpath, const T& console)
{
    printf("typename be call!\n");
    console->info(fpath.c_str());
    return 1;
}

// template <void* p = 0>
// inline int tf_defaultparam(std::string fpath, void* console = p)
// {
//     printf("LConsole* be call!\n");
//     // console->info(fpath.c_str());
//     return 1;
// }

template <int p = 0>
inline int tf_defaultparam(std::string fpath, const int& i = p)
{
    // console->info(fpath.c_str());
    printf("int be call!\n");
    printf(fpath.c_str());
    return 1;
}

#endif





