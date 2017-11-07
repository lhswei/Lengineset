#include <iostream>
#include <memory.h>
#include "lpublic.h"
#include "tcp_clienttest.h"

LTcpClientTest::LTcpClientTest(const char* szIPAress, int nPort)
{
    memset(m_szIPAdress, 0, _TCPTEST_CHAR_IP_LEN);
    if (szIPAress && strlen(szIPAress) > 0)
    {
        strncpy(m_szIPAdress, szIPAress, _TCPTEST_CHAR_IP_LEN - 1);
    }

    m_nPort = _TCPTEST_DEFAULT_SERVER_PORT;
    if (nPort > 1024 && nPort < 65534)
    {
        m_nPort = nPort;
    }

    m_socketConn = -1;
}

LTcpClientTest::~LTcpClientTest()
{
    Unit();
}

int LTcpClientTest::Init()
{
    int nResult = 0;
    int nRetCode = 0;

    LU_PROCESS_ERROR(m_szIPAdress[0]);
    LU_PROCESS_ERROR(m_nPort > 1024 && m_nPort < 65534);

    m_socketConn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    LU_PROCESS_ERROR(m_socketConn >= 0);

    memset(&m_server_addr, 0, sizeof(m_server_addr));
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(m_nPort);
    nRetCode = inet_pton(AF_INET, m_szIPAdress, &m_server_addr.sin_addr);
    LU_PROCESS_ERROR(nRetCode > 0);

    nResult = 1;
Exit0:

    return nResult;
}

void LTcpClientTest::Unit()
{
    if (m_socketConn >= 0)
    {
        close(m_socketConn);
        m_socketConn = -1;
    }
}

int LTcpClientTest::Connect2Server()
{
    int nResult = 0;
    int nRetCode = 0;

    nRetCode = connect(m_socketConn, (sockaddr*)&m_server_addr, sizeof(m_server_addr));
    LU_PROCESS_ERROR(nRetCode >= 0);

    nResult = 1;
Exit0:
    printf("tcpserver Connect2Server result = %d\n", nResult);
    return nResult;
}

void LTcpClientTest::Run()
{
    char szMessage[_TCPTEST_MAX_BUFFER_SIZE];
    int nRetCode = 0;
    while(true)
    {
        printf("send message: ");
        memset(szMessage, 0, _TCPTEST_MAX_BUFFER_SIZE);
        fgets(szMessage, _TCPTEST_MAX_BUFFER_SIZE - 1, stdin);
        nRetCode = send(m_socketConn, szMessage, strlen(szMessage), 0);
        printf("send result = %d\n", nRetCode);
        L_SLEEP(1);
    }
}

int main(int argc, char* argv[])
{

    LTcpClientTest tcpClient("127.0.0.1", _TCPTEST_DEFAULT_SERVER_PORT);
    if (tcpClient.Init() == 1 && tcpClient.Connect2Server() == 1)
    {
        tcpClient.Run();
    }

    return 1;
}








