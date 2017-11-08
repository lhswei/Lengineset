#include "lpublic.h"
#include "redisnoblock.h"

LRedisNOBlock::LRedisNOBlock()
{
    m_szIPAdress[0] = '\0';
    m_szPassword[0] = '\0';
    m_nPort = 0;
    m_nDBNum = 0;
    m_pRedisContext = NULL;
    m_nUnSubscribe = 0;
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
    redisReply* pReply =  NULL;
    int wdone = 0;

    LU_PROCESS_SUCCESS(m_pRedisContext);
    LU_PROCESS_ERROR(m_nPort > 1024 && m_nPort < 65534);

    m_pRedisContext = redisConnectNonBlock(m_szIPAdress, m_nPort);
    LU_PROCESS_ERROR(m_pRedisContext);
    LU_PROCESS_ERROR(!m_pRedisContext->err);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "AUTH %s", m_szPassword);
    _R_FREE_REPLY_OBJECT(pReply);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "SELECT %d", m_nDBNum);
    _R_FREE_REPLY_OBJECT(pReply);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "SUBSCRIBE messagechat");
    _R_FREE_REPLY_OBJECT(pReply);

    pReply = (redisReply*)redisCommand(m_pRedisContext, "SUBSCRIBE messagechat1");
    _R_FREE_REPLY_OBJECT(pReply);

    while (!wdone)
    {
        redisBufferWrite(m_pRedisContext, &wdone);
    }
Exit1:
    nResult = 1;
Exit0:
    _R_FREE_REPLY_OBJECT(pReply);
    if (nResult != 1)
    {
        printf("[LRedisNOBlock] _connect ipadress = %s port = %d faild!\n", m_szIPAdress, m_nPort);
    }
    else
    {
        printf("[LRedisNOBlock] _connect ipadress = %s port = %d success!\n", m_szIPAdress, m_nPort);
    }
    return nResult;
}

void LRedisNOBlock::Breath()
{
    redisReply* pReply =  NULL;
    int wdone = 0;
    
    if (m_nUnSubscribe == 1)
    {
        m_nUnSubscribe = 0;
        pReply = (redisReply*)redisCommand(m_pRedisContext, "UNSUBSCRIBE messagechat");
        _R_FREE_REPLY_OBJECT(pReply);
    }

    pReply = (redisReply*)redisCommand(m_pRedisContext, "PING");
    _R_FREE_REPLY_OBJECT(pReply);

    while (!wdone)
    {
        redisBufferWrite(m_pRedisContext, &wdone);
    }

    while ((pReply = TryGetReply()) != NULL)
    {
        ProcessPing(pReply);
        ProcessChannelMsg(pReply);
        _R_FREE_REPLY_OBJECT(pReply);
    }
}

redisReply* LRedisNOBlock::TryGetReply()
{
    redisReply* pReply = NULL;
    LU_PROCESS_ERROR(redisBufferRead(m_pRedisContext) != REDIS_ERR);
    redisGetReplyFromReader(m_pRedisContext, (void**)&pReply);
Exit0:
    return pReply;
}

void LRedisNOBlock::ProcessPing(redisReply* pReply)
{
    const char* pStr = NULL;

    LU_PROCESS_ERROR(pReply);
    if(pReply->type == REDIS_REPLY_ARRAY && pReply->elements > 0 && pReply->element[0]->type == REDIS_REPLY_STRING)
    {
        pStr = pReply->element[0]->str;
    }
    else if (pReply->type == REDIS_REPLY_STATUS)
    {
        pStr = pReply->str;
    }

    if (strncmp(pStr, "PONG", 4) == 0 ||
        strncmp(pStr, "pong", 4) == 0)
    {
        printf("recv redis pong\n");
    }
Exit0:
    return;
}

void LRedisNOBlock::ProcessChannelMsg(redisReply* pReply)
{
    if (pReply && pReply->type == REDIS_REPLY_ARRAY &&
        pReply->elements > 0 && pReply->element[0]->type == REDIS_REPLY_STRING)
    {
        if (strncmp(pReply->element[0]->str, "MESSAGE", 7) == 0 ||
        strncmp(pReply->element[0]->str, "message", 7) == 0)
        {
            if (pReply->elements > 2)
            {
                printf("recv redis message: channel=%s data=%s\n", pReply->element[1]->str, pReply->element[2]->str);
                m_nUnSubscribe = 1;
            }
        }
        else if (strncmp(pReply->element[0]->str, "SUBSCRIBE", 9) == 0 ||
        strncmp(pReply->element[0]->str, "subscribe", 9) == 0)
        {
            if (pReply->elements > 1)
            {
                printf("redis subscribe success: channel=%s\n", pReply->element[1]->str);
            }
        }
        else if (strncmp(pReply->element[0]->str, "UNSUBSCRIBE", 11) == 0 ||
        strncmp(pReply->element[0]->str, "unsubscribe", 11) == 0)
        {
            if (pReply->elements > 1)
            {
                printf("redis UNSUBSCRIBE success: channel=%s\n", pReply->element[1]->str);
            }
        }
    }
}

