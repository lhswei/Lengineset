#include "lpublic.h"
#include "redisnoblock.h"

LRedisNOBlock::LRedisNOBlock()
{
    m_szIPAdress[0] = '\0';
    m_szPassword[0] = '\0';
    m_nPort = 0;
    m_nDBNum = 0;
    m_pRedisContext = NULL;
}

LRedisNOBlock::~LRedisNOBlock()
{
    Uninit();
}

void LRedisNOBlock::Uninit()
{
    _FreeRes();
    m_szIPAdress[0] = '\0';
    m_szPassword[0] = '\0';
    m_nPort = 0;
    m_nDBNum = 0;
}

int LRedisNOBlock::_FreeRes()
{
    _R_FREE_CONTEXT_OBJECT(m_pRedisContext);
    return 1;
}

int LRedisNOBlock::Init(const char* szIPAdress, int nPort, int nDBNum, const char* szPassword)
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


int LRedisNOBlock::Connect2Redis()
{
    int nResult = 0;
    timeval timeout;
    redisReply* pReply =  NULL;

    LU_PROCESS_SUCCESS(m_pRedisContext);
    LU_PROCESS_ERROR(m_nPort > 1024 && m_nPort < 65534);

    m_pRedisContext = redisConnectNonBlock(m_szIPAdress, m_nPort);
    LU_PROCESS_ERROR(m_pRedisContext);
    LU_PROCESS_ERROR(!m_pRedisContext->err);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "AUTH %s", m_szPassword);
    _R_FREE_REPLY_OBJECT(pReply);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "SELECT %d", m_nDBNum);
    _R_FREE_REPLY_OBJECT(pReply);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "SUBSCRIBE message");
    _R_FREE_REPLY_OBJECT(pReply);
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

void LRedisNOBlock::Breath()
{
    redisReply* pReply =  NULL;
    redisGetReply(m_pRedisContext, (void*)&pReply);
    if (pReply)
    {
        if (pReply->type == REDIS_REPLY_STRING)
        {
            printf("command: %s\n", pReply->str);
        }
        for(int i = 1; i < pReply->elements; i++)
        {
            redisReply* pElement = pReply->element[i];
            if (pElement && pElement->type == REDIS_REPLY_STRING)
            {
                printf("arg %d : %s\n", i, pReply->str);
            }
        }
    }
}
