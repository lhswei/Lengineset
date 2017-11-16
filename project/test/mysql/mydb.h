
#ifndef _MY_DB_H_
#define _MY_DB_H_

#include <mysql.h>

#define _MYDB_CHAR_IP_LEN           64
#define _MYDB_CHAR_COMM_LEN         128

#define _MYDB_QUERY_SQL_LEN         1024
#define _MYDB_QUERY_SQL_LEN2         (_MYDB_QUERY_SQL_LEN*2)

#define _MYDB_FREE_RESULT(pObj)     \
do                                  \
{                               \
if(pObj)                   \
{                           \
mysql_free_result((pObj));\
(pObj) = NULL;          \
}                           \
}while(false)

#define _MYDB_CLOSE_CONN(pObj)     \
do                                  \
{                               \
if (pObj)                   \
{                           \
mysql_close((pObj));\
(pObj) = NULL;          \
}                           \
}while(false)

class MyDB 
{
public:
    MyDB(const char *host, const char *user, const char *passwd, const char *db);
    ~MyDB();

    int Init();
    void UnInit();
    MYSQL_RES *Query(const char *pSql);
    void PrintResult(MYSQL_RES *pRes) const;
private:
    char        m_szHost[_MYDB_CHAR_IP_LEN];
    char        m_szUer[_MYDB_CHAR_COMM_LEN];
    char        m_szPasswd[_MYDB_CHAR_COMM_LEN];
    char        m_szDB[_MYDB_CHAR_COMM_LEN];
    MYSQL       *m_pMysqlConn;
};












#endif //_MY_DB_H_
