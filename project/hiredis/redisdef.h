// 
// @filename:    redisdef.h 
// @Author:      luhengsi
// @DateTime:    2017-11-03 20:27:23
// @Description: redisclient define
// @Description: 

#ifndef _LU_REDIS_DEF_H_
#define _LU_REDIS_DEF_H_

#ifdef __linux
#include <unistd.h>
#else
#include <synchapi.h >
#endif

#define _R_NAME_LEN               32
#define _R_LONG_NAME_LEN          64
#define _R_CHAR_IP_LEN            33
#define _R_CHAR_PASSWORD_LEN      32

#ifndef L_SLEEP(s)
#ifdef __linux
#define L_SLEEP(s) usleep((s)*1000)
#else
#define L_SLEEP(s) Sleep((s))
#endif //__linux
#endif //L_SLEEP(S)

#define _R_FREE_CONTEXT_OBJECT(pObj)     \
do                                  \
{                               \
if (pObj)                   \
{                           \
redisFree((pObj));\
(pObj) = NULL;          \
}                           \
}while(false)

#define _R_FREE_ASYNC_CONTEXT_OBJECT(pObj)     \
do                                  \
{                               \
if (pObj)                   \
{                           \
redisAsyncFree((pObj));\
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

#define _R_FREE_EVENT_OBJECT(pObj)     \
do                                  \
{                               \
if (pObj)                   \
{                           \
event_base_dispatch((pObj));\
(pObj) = NULL;          \
}                           \
}while(false)


#endif