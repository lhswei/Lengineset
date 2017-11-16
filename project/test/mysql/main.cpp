#include <stdio.h>
#include <iostream>
#include <memory.h>
#include <string.h>
#include "mydb.h"

int main(int argc, char *argv[])
{
    char szBuff[_MYDB_QUERY_SQL_LEN];
    MyDB mydb("10.20.73.86", "myroot", "1234", "kingeyes");
    if (mydb.Init() == 1)
    {
        MYSQL_RES *pRes = mydb.Query("SHOW VARIABLES LIKE \'character_set_server\';");
        mydb.PrintResult(pRes);
        _MYDB_FREE_RESULT(pRes);

        pRes = mydb.Query("SHOW VARIABLES LIKE \'character_set_database\';");
        mydb.PrintResult(pRes);
        _MYDB_FREE_RESULT(pRes);

        while(true)
        {
            printf("---SQL QUERY STATEMENT:---\n");
            memset(szBuff, 0, _MYDB_QUERY_SQL_LEN);
            fgets(szBuff, _MYDB_QUERY_SQL_LEN - 1, stdin);
            if (strncmp(szBuff, "quit", 4) == 0)
                break;

            int nLen = strlen(szBuff);
            if (nLen > 1)
            {   
                szBuff[nLen - 1] = ' ';
                pRes = mydb.Query(szBuff);
                mydb.PrintResult(pRes);
                _MYDB_FREE_RESULT(pRes);
            }
        }
    }

    return 1;
}


