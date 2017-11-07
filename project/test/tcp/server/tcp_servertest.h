// 
// @filename:    tcp_servertest.h  
// @Author:      luhengsi
// @DateTime:    2017-11-07 20:27:23
// @Description: tcp server
// @Description: 

#ifndef _LU_TCP_SERVER_TEST_H_
#define _LU_TCP_SERVER_TEST_H_

#include <stdio.h>
#include <stdlib.h>  
#include <stddef.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <map>
#include "../tcp_testdef.h"

struct LClientConnInfo 
{
    int     m_socketConn;
    char    m_szIPAdress[_TCPTEST_CHAR_IP_LEN];
};

typedef std::map<int, LClientConnInfo*>     LU_CLIENT_CONN_MAP;

class LTcpServerTest 
{
public:
    LTcpServerTest(int nPort);
    ~LTcpServerTest();

    int Init();
    void UnInit();
    void Breath();
    void Run();

private:
    int                     m_socketServer;
    int                     m_nPort;
    sockaddr_in             m_addrServer;
    LU_CLIENT_CONN_MAP      m_mapClientConn;
};















#endif //_LU_TCP_SERVER_TEST_H_


