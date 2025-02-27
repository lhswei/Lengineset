#include "redisclient.h"
#include "lpublic.h"

LRedisClient::LRedisClient()
{
    m_szIPAdress[0] = '\0';
    m_szPassword[0] = '\0';
    m_nPort = 0;
    m_nDBNum = 0;
    m_pRedisContext = NULL;
}

LRedisClient::~LRedisClient()
{
    Uninit();
}

void LRedisClient::Uninit()
{
    _FreeRes();
    m_szIPAdress[0] = '\0';
    m_szPassword[0] = '\0';
    m_nPort = 0;
    m_nDBNum = 0;
}

int LRedisClient::_FreeRes()
{
    _R_FREE_CONTEXT_OBJECT(m_pRedisContext);
    return 1;
}

int LRedisClient::Init(const char* szIPAdress, int nPort, int nDBNum, const char* szPassword)
{
    int nResult = 0;

    LU_PROCESS_ERROR(szIPAdress);
    LU_PROCESS_ERROR(szPassword);
    LU_PROCESS_ERROR(nPort > 1024 && nPort < 65534);

    m_nDBNum = nDBNum;
    m_nPort = nPort;
    
    memset(m_szIPAdress, 0, _R_CHAR_IP_LEN);
    strncpy(m_szIPAdress, szIPAdress, _R_CHAR_IP_LEN - 1);

    memset(m_szPassword, 0, _R_CHAR_PASSWORD_LEN);
    strncpy(m_szPassword, szPassword, _R_CHAR_PASSWORD_LEN - 1);

    nResult = 1;
Exit0:

    return nResult;
}

int LRedisClient::_Connect()
{
    int nResult = 0;
    timeval timeout;
    LU_PROCESS_SUCCESS(m_pRedisContext);
    LU_PROCESS_ERROR(m_nPort > 1024 && m_nPort < 65534);

    //m_pRedisContext = redisConnect(m_szIPAdress, m_nPort);
    timerclear(&timeout);
    timeout.tv_usec = 500000; // 0.5 seconds    
    m_pRedisContext = redisConnectWithTimeout(m_szIPAdress, m_nPort, timeout);
    LU_PROCESS_ERROR(m_pRedisContext);
    LU_PROCESS_ERROR(!m_pRedisContext->err);

Exit1:
    nResult = 1;
Exit0:
    if (nResult != 1)
    {
        printf("[LRedisClient] _connect ipadress = %s port = %d faild!\n", m_szIPAdress, m_nPort);
    }
    else
    {
        printf("[LRedisClient] _connect ipadress = %s port = %d success!\n", m_szIPAdress, m_nPort);
    }
    return nResult;
}

int LRedisClient::_Auth()
{
    int nResult = 0;
    redisReply* pReply =  NULL;
    
    LU_PROCESS_ERROR(m_szPassword);
    LU_PROCESS_ERROR(m_pRedisContext);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "AUTH %s", m_szPassword);
    LU_PROCESS_ERROR(pReply);

    nResult = 1;
Exit0:
    _R_FREE_REPLY_OBJECT(pReply);
    if (nResult != 1)
    {
        printf("[LRedisClient] _Auth faild!\n");
    }
    else
    {
        printf("[LRedisClient] _Auth success!\n");
    }
    return nResult;
}

int LRedisClient::_SelectDB()
{
    int nResult = 0;
    redisReply* pReply =  NULL;
    
    LU_PROCESS_ERROR(m_szPassword);
    LU_PROCESS_ERROR(m_pRedisContext);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "SELECT %d", m_nDBNum);
    LU_PROCESS_ERROR(pReply);

    nResult = 1;
Exit0:
    _R_FREE_REPLY_OBJECT(pReply);
    if (nResult != 1)
    {
        printf("[LRedisClient] _SelectDB %d faild!\n", m_nDBNum);
    }
    else
    {
        printf("[LRedisClient] _SelectDB [db:%d] success!\n", m_nDBNum);
    }
    return nResult;
}

int LRedisClient::Connect2Redis()
{
	int nResult = 0;
    
    nResult = _Connect();
    LU_PROCESS_ERROR(nResult == 1);
    
    nResult = _Auth();
    LU_PROCESS_ERROR(nResult == 1);
    
    nResult  = _SelectDB();
    LU_PROCESS_ERROR(nResult == 1);

    nResult = 1;
 Exit0:
    if (nResult != 1)
    {
        printf("[LRedisClient] Connect2Redis error, code=%d", nResult);
        _FreeRes();
    }
    
    return nResult;
}

int LRedisClient::ReConnect2Redis()
{
	int nResult = 0;
    
    _FreeRes();
    nResult = Connect2Redis();
    LU_PROCESS_ERROR(nResult == 1);
    
    nResult = 1;
Exit0:
    if (nResult != 1)
    {
        printf("[LRedisClient] ForceReconnect2Redis error, code=%d", nResult);
    }
    return nResult;
}

int LRedisClient::SetString(const char* szKey, const char* szValue)
{
	int            nResult  = 0;
	redisReply* pReply = NULL;

	LU_PROCESS_ERROR(szKey);
	LU_PROCESS_ERROR(szValue);
	
	pReply = (redisReply*)redisCommand(m_pRedisContext, "SET %s %s", szKey, szValue);
	LU_PROCESS_ERROR(pReply);
	LU_PROCESS_ERROR(pReply->type != REDIS_REPLY_ERROR);
	nResult = 1;
Exit0:
	_R_FREE_REPLY_OBJECT(pReply);
	return nResult; 
}

int LRedisClient::GetString(const char* szKey, char* szValueOut, int nOutLen)
{
	int            nResult  = 0;
	redisReply*	   pReply = NULL;

	LU_PROCESS_ERROR(szKey);
	LU_PROCESS_ERROR(szValueOut);
	LU_PROCESS_ERROR(nOutLen > 0);

	pReply = (redisReply*)redisCommand(m_pRedisContext, "GET %s", szKey);

	LU_PROCESS_ERROR(pReply);
	LU_PROCESS_ERROR(pReply->type == REDIS_REPLY_STRING);
    LU_PROCESS_ERROR(nOutLen > pReply->len);
    
    memset(szValueOut, 0, nOutLen);
	strncpy(szValueOut, pReply->str, nOutLen - 1);
    
	nResult = 1;
Exit0:
	_R_FREE_REPLY_OBJECT(pReply);
	return nResult;
}

int LRedisClient::HSetByte(const char*szModule, const char* szKey, void* pBuffer, int nBufferLen)
{
	int            nResult  = 0;
	redisReply*	   pReply = NULL;

    LU_PROCESS_ERROR(szModule);
	LU_PROCESS_ERROR(szKey);
	LU_PROCESS_ERROR(pBuffer);
	LU_PROCESS_ERROR(nBufferLen > 0);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "HSET %s %s %b", szModule, szKey, pBuffer, nBufferLen);
	LU_PROCESS_ERROR(pReply);
	LU_PROCESS_ERROR(pReply->type != REDIS_REPLY_ERROR);
	nResult = 1;
Exit0:
	_R_FREE_REPLY_OBJECT(pReply);
	return nResult;
}

int LRedisClient::HGetByte(const char*szModule, const char* szKey, void* pBuffer, int nBufferLen)
{
	int            nResult  = 0;
	redisReply*	   pReply = NULL;
    int            nLen     = 0;
    LU_PROCESS_ERROR(szModule);
	LU_PROCESS_ERROR(szKey);
	LU_PROCESS_ERROR(pBuffer);
	LU_PROCESS_ERROR(nBufferLen > 0);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "HGET %s %s", szModule, szKey);
	LU_PROCESS_ERROR(pReply);
	LU_PROCESS_ERROR(pReply->type == REDIS_REPLY_STRING);
    nLen = pReply->len >= nBufferLen ? nBufferLen : pReply->len;
    memcpy(pBuffer, pReply->str, nLen);

	nResult = nLen;
Exit0:
	_R_FREE_REPLY_OBJECT(pReply);
	return nResult;
}

int LRedisClient::SetByte(const char* szKey, void* pBuffer, int nBufferLen)
{
	int            nResult  = 0;
	redisReply*	   pReply = NULL;
    char *cmd;
    int len = 0;

	LU_PROCESS_ERROR(szKey);
	LU_PROCESS_ERROR(pBuffer);
	LU_PROCESS_ERROR(nBufferLen > 0);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "SET %s %b", szKey, pBuffer ,(size_t)nBufferLen);
	LU_PROCESS_ERROR(pReply);
	LU_PROCESS_ERROR(pReply->type != REDIS_REPLY_ERROR);
	nResult = 1;
Exit0:
	_R_FREE_REPLY_OBJECT(pReply);
	return nResult;
}


int LRedisClient::GetByte(const char* szKey, void* pBuffer, int nBufferLen)
{
	int            nResult  = 0;
	redisReply*	   pReply = NULL;
    int            nLen     = 0;

	LU_PROCESS_ERROR(szKey);
	LU_PROCESS_ERROR(pBuffer);
	LU_PROCESS_ERROR(nBufferLen > 0);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "GET %s", szKey);
	LU_PROCESS_ERROR(pReply);
	LU_PROCESS_ERROR(pReply->type == REDIS_REPLY_STRING);
    nLen = pReply->len >= nBufferLen ? nBufferLen : pReply->len;
    memcpy(pBuffer, pReply->str, nLen);

	nResult = nLen;
Exit0:
	_R_FREE_REPLY_OBJECT(pReply);
	return nResult;
}
