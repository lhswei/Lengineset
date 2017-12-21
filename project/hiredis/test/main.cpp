
#include <stdio.h>  
#include <stdlib.h>  
#include <stddef.h> 
#include "../redisclient.h"
#include "../redisasyncclient.h"
#include "../redisnoblock.h"

struct ST_TEST
{
    int a;
    char ch[16];
    int b;
};

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

            int nLent = sizeof(ST_TEST);

            // ST_TEST st;
            
            // memset(&st, 0, nLent);

            // st.a = 7;
            // st.b = 5;
            // sprintf(st.ch, "hellow");

            // void *pBuffer = static_cast<void*>(&st);
            // nRetCode = pRedisClient->HSetByte("module1", "luhengsi1", pBuffer, nLent);
            // printf("HSetByte nRetCode = %d\n", nRetCode);

            // nRetCode = pRedisClient->SetByte("bytetest1", pBuffer, nLent);
            // printf("SetByte nRetCode = %d\n", nRetCode);

            // ST_TEST st1;
            // void *pBuffer1 = static_cast<void*>(&st1);
            // memset(pBuffer1, 0, nLent);
            // int nGetLen = pRedisClient->HGetByte("module1", "luhengsi1", pBuffer1, nLent);
            // printf("HGetByte: nGetLen = %d, a = %d, b = %d, ch = %s\n", nGetLen, st1.a, st1.b, st1.ch);
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
