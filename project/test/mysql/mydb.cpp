#include <lpublic.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "mydb.h"

MyDB::MyDB(const char *host, const char *user, const char *passwd, const char *db)
{

    m_pMysqlConn = NULL;

    memset(m_szHost, 0, _MYDB_CHAR_IP_LEN);
    memset(m_szUer, 0, _MYDB_CHAR_COMM_LEN);
    memset(m_szPasswd, 0, _MYDB_CHAR_COMM_LEN);
    memset(m_szDB, 0, _MYDB_CHAR_COMM_LEN);

    if (host)
        strncpy(m_szHost, host, _MYDB_CHAR_IP_LEN - 1);
        
    if (user)
        strncpy(m_szUer, user, _MYDB_CHAR_COMM_LEN - 1);

    if (passwd)
        strncpy(m_szPasswd, passwd, _MYDB_CHAR_COMM_LEN - 1);

    if (db)
        strncpy(m_szDB, db, _MYDB_CHAR_COMM_LEN - 1);
}

MyDB::~MyDB()
{
    UnInit();
}

void MyDB::UnInit()
{
    _MYDB_CLOSE_CONN(m_pMysqlConn);
}

int MyDB::Init()
{
    int nResult = 0;
    MYSQL *pRetSql = NULL;
    _MYDB_CLOSE_CONN(m_pMysqlConn);

    m_pMysqlConn = mysql_init(NULL);
    LU_PROCESS_ERROR(m_pMysqlConn);

    pRetSql = mysql_real_connect(m_pMysqlConn, m_szHost, m_szUer, m_szPasswd, m_szDB, 0, NULL, 0);
    LU_PROCESS_ERROR(pRetSql);

    nResult = 1;
Exit0:
    if(nResult != 1)
    {
        printf("[MyDB]: init result = %d\n", nResult);
    }
    return nResult;
}

MYSQL_RES *MyDB::Query(const char *pSql)
{
    MYSQL_RES *pRes = NULL;
    int nRetCode = -1;
    int nLen = 0;
    LU_PROCESS_ERROR(m_pMysqlConn);

    nLen = strlen(pSql);
    LU_PROCESS_ERROR(pSql && nLen < _MYDB_QUERY_SQL_LEN);

    nRetCode = mysql_real_query(m_pMysqlConn, pSql, nLen);
    LU_PROCESS_ERROR(nRetCode == 0);

    pRes = mysql_store_result(m_pMysqlConn);
    LU_PROCESS_ERROR(pRes);

Exit0:
    printf("[MyDB]: query code = %d\n", nRetCode);

    if(!pRes && m_pMysqlConn)
    {
        printf("[MyDB]: query err %s\n", mysql_error(m_pMysqlConn));
    }

    return pRes;
}

void MyDB::PrintResult(MYSQL_RES *pRes) const
{
    int nNumRows = 0;
    int nNumFields = 0;
    MYSQL_ROW MysqlRow;
    printf("------------------start print result-----------------------\n");
    LU_PROCESS_ERROR(pRes);

    nNumRows = mysql_num_rows(pRes);
    nNumFields = mysql_num_fields(pRes);

    while((MysqlRow = mysql_fetch_row(pRes)))
    {
        for(int i = 0; i < nNumFields; ++i)
        {
            printf("%s\t", MysqlRow[i] ? MysqlRow[i] : "NULL");
        }
        printf("\n");
    }

Exit0:
    printf("table info: rows = %d, fileds = %d\n", nNumRows, nNumFields);
    printf("------------------start print end-----------------------\n");
    return;
}



