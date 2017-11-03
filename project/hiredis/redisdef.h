// 
// @filename:    redisdef.h 
// @Author:      luhengsi
// @DateTime:    2017-11-03 20:27:23
// @Description: redisclient define
// @Description: 

#ifndef _LU_REDIS_DEF_H_
#define _LU_REDIS_DEF_H_

#define _R_NAME_LEN               32
#define _R_LONG_NAME_LEN          64
#define _R_CHAR_IP_LEN            33
#define _R_CHAR_PASSWORD_LEN      32

#define _R_FREE_CONTEXT_OBJECT(pObj)     \
do                                  \
{                               \
if (pObj)                   \
{                           \
redisFree((pObj));\
(pObj) = NULL;          \
}                           \
}while(false)

#define _R_FREE_REPLY_OBJECT(pObj)     \
do                                  \
{                               \
if (pObj)                   \
{                           \
freeReplyObject((pObj));\
(pObj) = NULL;          \
}                           \
}while(false)




#endif