extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "debugserver.h"
#include <stdexcept>      // std::logic_error

L_DEFINE_LUA_CLASS(DebugServerWrapper, DebugServer);

void DebugServer::AcceptThread()
{
    if (CheckRunState(DBG_RUN_STATE::DGB_ATTACH))
        return;

    if (m_nSocketClient > 0)
    {
        ::closesocket(m_nSocketClient);
        m_nSocketClient = INVALID_SOCKET;
    }

    StopThread();

    auto fThread = [this]() -> void {
        while (this->m_nSocketClient < 0 && this->m_nSocketLisent >= 0)
        {
            sockaddr_in sockaddrClient;
            int nRetCode = 0;
            int nsin_size = sizeof(struct sockaddr_in);
            this->SetRunState(DBG_RUN_STATE::DBG_DETACH);
            this->m_nSocketClient = ::accept(this->m_nSocketLisent, (struct sockaddr *)&sockaddrClient, &nsin_size);
            if (this->m_nSocketClient >= 0)
            {
                this->SetRunState(DBG_RUN_STATE::DGB_ATTACH);
                unsigned long ul = 1;
                ::ioctlsocket(this->m_nSocketClient, FIONBIO, &ul);
                printf("debug> attach success.\n");
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    };
    m_pThread = new std::thread(fThread);
}

// debugserver
DebugServer::DebugServer()
:m_dbgstate(DBG_RUN_STATE::DBG_NONE),
m_pThread(new std::thread())
{

}

DebugServer::~DebugServer()
{
    UnInit();
}

int DebugServer::UnInit()
{
    if (m_nSocketClient > 0)
    {
        ::closesocket(m_nSocketClient);
        m_nSocketClient = INVALID_SOCKET;
    }

    if (m_nSocketLisent > 0)
    {
        ::closesocket(m_nSocketLisent);
        m_nSocketLisent = INVALID_SOCKET;
		//终止 DLL 的使用  
		::WSACleanup();
    }

    StopThread();
	return 1;
}

int DebugServer::StartServer(short port)
{
    int nRet = 0;
    if (port > 1024 && port < 65534)
    {
        m_nPort = port;
		printf("listening port =  %d\n", port);
        if (m_nSocketClient < 0)
        {
            InitServer();
            nRet = 1;
        }
        else
            printf("debug> listening socket already existed: %d\n", m_nSocketClient);
    }
    return nRet;
}

int DebugServer::StoprServer()
{
    int nRet = 0;

    return nRet;
}

void DebugServer::StopThread()
{
    if (m_pThread && m_pThread->joinable())
    {   
        m_pThread->join();
        delete m_pThread;
        m_pThread = nullptr;
    }
}

int DebugServer::Send(std::string msg)
{
    int nRet = 0;

    if (m_nSocketClient > 0 && !CheckRunState(DBG_RUN_STATE::DGB_ATTACH))
    {
        return 0;
    }

    if(::send(m_nSocketClient, msg.c_str(), msg.length(), 0) > 0)
    {
        nRet = 1;
    }
    return nRet;
}


int DebugServer::Recv(std::string& msg)
{
    int nRet = 0;
    if (m_nSocketClient > 0 && !CheckRunState(DBG_RUN_STATE::DGB_ATTACH))
    {
        return 0;
    }
    char chbuffer[2048] = {0};
    if (::recv(m_nSocketClient, chbuffer, 2047, 0) > 0)
    {
        msg.append(chbuffer);
        nRet = 1;
    }
    return nRet;
}

int DebugServer::InitServer()
{
    int nRet = 0;
    
    if (!CheckRunState(DBG_RUN_STATE::DBG_NONE))
        return nRet;
    //初始化 DLL  
	WSADATA wsaData;
	::WSAStartup(MAKEWORD(2, 2), &wsaData);

    m_nSocketLisent = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_nSocketLisent >= 0)
    {   
        sockaddr_in addrServer;
        memset(&addrServer, 0, sizeof(addrServer));
        addrServer.sin_family = AF_INET;
        addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
        addrServer.sin_port = htons(m_nPort);
        if (::bind(m_nSocketLisent, (sockaddr *)&addrServer, sizeof(addrServer)) >= 0)
        {
            if (::listen(m_nSocketLisent, _TCP_MAX_CONN_WAIT) >= 0)
            {
                AcceptThread();
                nRet = 1;
            }
        }
    }
    return nRet;
}

int DebugServer::Dettach()
{
    int nRet = 0;
    if (CheckRunState(DBG_RUN_STATE::DGB_ATTACH) && m_nSocketClient >= 0)
    {
        ::closesocket(m_nSocketClient);
        m_nSocketClient = INVALID_SOCKET;
        SetRunState(DBG_RUN_STATE::DBG_DETACH);
        AcceptThread();
        nRet = 1;
    }

    return nRet;
}

void DebugServer::SetRunState(DBG_RUN_STATE st)
{
    try
    {
        // using a local lock_guard to lock mtx guarantees unlocking on destruction / exception:
        std::lock_guard<std::mutex> lck(this->m_mtx);
        m_dbgstate = st;
    }
    catch (std::logic_error &)
    {
        printf("[exception caught]\n");
    }
}

bool DebugServer::CheckRunState(DBG_RUN_STATE st)
{
    try
    {
        // using a local lock_guard to lock mtx guarantees unlocking on destruction / exception:
        std::lock_guard<std::mutex> lck(this->m_mtx);
        return m_dbgstate == st;
    }
    catch (std::logic_error &)
    {
        printf("[exception caught]\n");
    }
    return false;
}

// lua interface
DebugServerWrapper::DebugServerWrapper(lua_State* L):
L_INHERIT_TYPE(DebugServer)(L)
{

}

DebugServerWrapper::~DebugServerWrapper()
{

}

int DebugServerWrapper::StartServer(lua_State* L)
{
    int nRet = 0;
    int port = luaL_checkinteger(L, 1);
    if (port > 1024 && port < 65534)
    {
        nRet = superclass::StartServer(port);
    }
    lua_pushinteger(L, nRet);
    return nRet;
}

int DebugServerWrapper::StoprServer(lua_State* L)
{
    int nRet = 0;
    nRet = superclass::StoprServer();
    lua_pushinteger(L, nRet);
    return 1;
}

int DebugServerWrapper::Send(lua_State* L)
{
    int nRet = 0;
    const char* msg = luaL_checkstring(L, 1);
    if (msg)
    {
        nRet = superclass::Send(std::string(msg));
    }
    lua_pushinteger(L, nRet);
    return 1;
}

int DebugServerWrapper::Recv(lua_State* L)
{
    int nRet = 0;
    std::string msg;
    int len = superclass::Recv(msg);
    if (len > 0)
    {
        lua_pushstring(L, msg.c_str());
        nRet++;
    }
    return nRet;
}

L_REG_TYPE(DebugServerWrapper) DebugServerWrapper::Functions[] =
{
    {"StartServer", &DebugServerWrapper::StartServer},
    {"StoprServer", &DebugServerWrapper::StoprServer},
    {"Send", &DebugServerWrapper::Send},
    {"Revc", &DebugServerWrapper::Recv},
    {NULL, NULL}
};



L_DEFINE_LUA_CLASS(MyTestWrapper, MyTest);

int MyTestWrapper::LuaFoo(lua_State* L)
{
	int port = luaL_checkinteger(L, 1);
	Foo(port);
	return 1;
}

L_REG_TYPE(MyTestWrapper) MyTestWrapper::Functions[] =
{
	{ "Foo", &MyTestWrapper::LuaFoo },
	{NULL, NULL}
};

