// 
// @filename:    redisclient.h 
// @Author:      luhengsi
// @DateTime:    2017-11-03 20:27:23
// @Description: redisclient connect to redis
// @Description: 

#ifndef _LREDIS_CLIENT_H_
#define _LREDIS_CLIENT_H_

#include <stdio.h>  
#include <stdlib.h>  
#include <stddef.h>  
#include <stdarg.h>  
#include <string.h>  
#include <assert.h>  
#include <hiredis/hiredis.h> 
#include "redisdef.h"


class LRedisClient 
{
public:
    LRedisClient();
    ~LRedisClient();

    int Init(const char* szIPAdress, int nPort, int nDBNum, const char* szPassword);
    void Uninit();
    int Connect2Redis();
    int ReConnect2Redis();

public:
    int SetString(const char* szKey, const char* szValue);
	int GetString(const char* szKey, char* szValueOut, int nOutLen);

    int HSetByte(const char*szModule, const char* szKey, void* pBuffer, int nBufferLen);
    int HGetByte(const char*szModule, const char* szKey, void* pBuffer, int nBufferLen);
    int SetByte(const char* szKey, void* pBuffer, int nBufferLen);
    int GetByte(const char* szKey, void* pBuffer, int nBufferLen);

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
    redisContext*               m_pRedisContext;
};

#endif
