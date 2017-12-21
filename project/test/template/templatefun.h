
#ifndef _TEMPLATE_FUN_H_
#define _TEMPLATE_FUN_H_

#include <string>
#include "templatedef.h"

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

#endif





