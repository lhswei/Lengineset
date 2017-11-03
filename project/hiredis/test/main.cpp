#include <stdio.h>
#include "../redisclient.h"

int main(int argc, char* argv[])
{
    LRedisClient* pRedisClient = new LRedisClient();
    if (pRedisClient)
    {
        int nRetCode = pRedisClient->Init("10.20.77.106", 6379, 2, "kingsoft");
        if (nRetCode == 1)
        {
            pRedisClient->Connect2Redis();
            char szOut[64] = {0};
            const char* szKey = "lhswei_4_test1";
            pRedisClient->GetString(szKey, szOut, 64);
            printf("%s = %d\n", szKey, szOut);
        }
    }
    return 0;
}
