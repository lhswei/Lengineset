#include <string.h> 
#include "redisasyncclient.h"
#include "lpublic.h"

void getCallback(redisAsyncContext *c, void *r, void *privdata) {
    redisReply *reply = (redisReply*)r;
    if (reply == NULL) return;
    printf("argv[%s]: %s\n", (char*)privdata, reply->str);

    /* Disconnect after receiving the reply to GET */
    redisAsyncDisconnect(c);
}

void connectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Connected...\n");
}

void disconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Disconnected...\n");
}


LRedisAsyncClient::LRedisAsyncClient()
{
    m_szIPAdress[0] = '\0';
    m_szPassword[0] = '\0';
    m_nPort = 0;
    m_nDBNum = 0;
    m_pRedisContext = NULL;
    m_pEventBase = NULL;
}

LRedisAsyncClient::~LRedisAsyncClient()
{
}

int LRedisAsyncClient::Init(const char *szIPAdress, int nPort, int nDBNum, const char *szPassword)
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

void LRedisAsyncClient::Uninit()
{
    _FreeRes();
    m_szIPAdress[0] = '\0';
    m_szPassword[0] = '\0';
    m_nPort = 0;
    m_nDBNum = 0;
}

int LRedisAsyncClient::_FreeRes()
{
    _R_FREE_ASYNC_CONTEXT_OBJECT(m_pRedisContext);
    _R_FREE_EVENT_OBJECT(m_pEventBase);
    return 1;
}

int LRedisAsyncClient::Connect2Redis()
{
    int nResult = 0;

    nResult = _Connect();
    LU_PROCESS_ERROR(nResult == 1);
    
    nResult = 1;
Exit0:
    return nResult;
}

int LRedisAsyncClient::ReConnect2Redis()
{
    int nResult = 0;

    return nResult;
}

int LRedisAsyncClient::_Connect()
{
    int nResult = 0;
    signal(SIGPIPE, SIG_IGN);
    LU_PROCESS_SUCCESS(m_pRedisContext);
    LU_PROCESS_ERROR(m_nPort > 1024 && m_nPort < 65534);

    if (!m_pEventBase)
    {
        m_pEventBase = event_base_new();
    }
    LU_PROCESS_ERROR(m_pEventBase);

    m_pRedisContext = redisAsyncConnect(m_szIPAdress, m_nPort);
    LU_PROCESS_ERROR(m_pRedisContext);
    LU_PROCESS_ERROR(!m_pRedisContext->err);

    redisLibeventAttach(m_pRedisContext,m_pEventBase);

    redisAsyncSetConnectCallback(m_pRedisContext,connectCallback);
    redisAsyncSetDisconnectCallback(m_pRedisContext,disconnectCallback);
    redisAsyncCommand(m_pRedisContext, NULL, NULL, "AUTH %s", m_szPassword);
    redisAsyncCommand(m_pRedisContext, NULL, NULL, "SELECT %d", m_nDBNum);
    redisAsyncCommand(m_pRedisContext, getCallback, (char*)"end-1", "GET lhswei_4_test1");
    _R_FREE_EVENT_OBJECT(m_pEventBase);
Exit1:
    nResult = 1;
Exit0:
    if (nResult != 1)
    {
        printf("[LRedisAsyncClient] _connect ipadress = %s port = %d faild!\n", m_szIPAdress, m_nPort);
    }
    else
    {
        printf("[LRedisAsyncClient] _connect ipadress = %s port = %d apply...\n", m_szIPAdress, m_nPort);
    }
    return nResult;
}

int LRedisAsyncClient::_Auth()
{
    int nResult = 0;

    return nResult;
}

int LRedisAsyncClient::_SelectDB()
{
    int nResult = 0;

    return nResult;
}


