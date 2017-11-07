// 
// @filename:    tcp_clienttest.h  
// @Author:      luhengsi
// @DateTime:    2017-11-07 20:27:23
// @Description: tcp client
// @Description: 

#ifndef _LU_TCP_CLIENT_TEST_H_
#define _LU_TCP_CLIENT_TEST_H_

#include <stdio.h>
#include <stdlib.h>  
#include <stddef.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../tcp_testdef.h"

class LTcpClientTest 
{
public:
    LTcpClientTest(const char* szIPAress, int nPort);
    ~LTcpClientTest();

    int Init();
    void Unit();
    int Connect2Server();

    void Run();

private:
    int             m_socketConn;
    char            m_szIPAdress[_TCPTEST_CHAR_IP_LEN];
    int             m_nPort;
    sockaddr_in     m_server_addr;
};


#endif