#ifdef __linux
#include <unistd.h>
#else
#include <synchapi.h >
#endif
#include <stdio.h>  
#include <stdlib.h>  
#include <stddef.h> 
#include "../redisclient.h"
#include "../redisasyncclient.h"
#include "../redisnoblock.h"

#ifndef L_SLEEP(s)
#ifdef __linux
#define L_SLEEP(s) usleep((s)*1000)
#else
#define L_SLEEP(s) Sleep((s))
#endif //__linux
#endif //L_SLEEP(S)

int main(int argc, char* argv[])
{
    LRedisClient* pRedisClient = new LRedisClient();
    if (pRedisClient)
    {
        int nRetCode = pRedisClient->Init("10.20.77.106", 6379, 2, "kingsoft");
        if (nRetCode == 1 && pRedisClient->Connect2Redis() == 1)
        {
            char szOut[64] = {0};
            const char* szKey = "lhswei_4_test1";
            pRedisClient->GetString(szKey, szOut, 64);
            printf("%s = %s\n", szKey, szOut);
        }

        delete pRedisClient;
        pRedisClient = NULL;
    }

    LRedisAsyncClient* pRedisAsyncClient = new LRedisAsyncClient();
    if (pRedisAsyncClient)
    {
        int nRetCode = pRedisAsyncClient->Init("10.20.77.106", 6379, 2, "kingsoft");
        if (nRetCode == 1)
        {
            pRedisAsyncClient->Connect2Redis();
        }
        // int nTick = 0;
        // while(nTick <= 60)
        // {
        //     nTick++;
        //     L_SLEEP(1000);
        // }
        delete pRedisAsyncClient;
        pRedisAsyncClient = NULL;
    }

    LRedisNOBlock* pRedisNOBlockClient = new LRedisNOBlock();
    if (pRedisNOBlockClient)
    {
        int nRetCode = pRedisNOBlockClient->Init("10.20.77.106", 6379, 2, "kingsoft");
        if (nRetCode == 1)
        {
            pRedisNOBlockClient->Connect2Redis();
        }
        int nTick = 0;
        while(nTick <= 60)
        {
            nTick++;
            pRedisNOBlockClient->Breath();
            L_SLEEP(1000);
        }
        delete pRedisNOBlockClient;
        pRedisNOBlockClient = NULL;
    }

    printf("press any key to countinue...\n");
    getchar();
    return 0;
}
