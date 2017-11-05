// 
// @filename:    redisasyncclient.h 
// @Author:      luhengsi
// @DateTime:    2017-11-03 20:27:23
// @Description: redisasyncclient connect to redis
// @Description: 

#ifndef _LREDIS_ASYNC_CLIENT_H_
#define _LREDIS_ASYNC_CLIENT_H_

#include <stdio.h>  
#include <stdlib.h>  
#include <stddef.h> 
#include "redisdef.h"
#include <hiredis/hiredis.h>
#include <hiredis/async.h>

#ifdef __linux
#ifndef SSIZE_T
typedef int SSIZE_T;
#endif
#include <time.h>
#include <signal.h>
#endif//__linux

#include <hiredis/adapters/libevent.h>

class LRedisAsyncClient 
{
public:
    LRedisAsyncClient();
    ~LRedisAsyncClient();

    int Init(const char* szIPAdress, int nPort, int nDBNum, const char* szPassword);
    void Uninit();
    int Connect2Redis();
    int ReConnect2Redis();

private:
    int _FreeRes();
    int _Connect();
    int _Auth();
    int _SelectDB();

private:
    char                        m_szIPAdress[_R_CHAR_IP_LEN];
    char                        m_szPassword[_R_CHAR_PASSWORD_LEN];
    int                         m_nPort;
    int                         m_nDBNum;
    redisAsyncContext*          m_pRedisContext;
    event_base*				    m_pEventBase;
};










#endif
