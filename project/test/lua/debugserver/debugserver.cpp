extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#include "debugserver.h"
#include <stdexcept>      // std::logic_error

void DebugServer::AcceptThread()
{
    if (m_pThread)
    {
        delete m_pThread;
        m_pThread = nullptr;
    }

    auto fThread = [this]() -> void {
        while (this->m_nSocketClient < 0)
        {
            try
            {
                // using a local lock_guard to lock mtx guarantees unlocking on destruction / exception:
                std::lock_guard<std::mutex> lck(this->m_mtx);
                sockaddr_in sockaddrClient;
                int nRetCode = 0;
                int nsin_size = sizeof(struct sockaddr_in);
                this->m_nSocketClient = accept(this->m_nSocketLisent, (struct sockaddr *)&sockaddrClient, &nsin_size);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            catch (std::logic_error &)
            {
                printf("[exception caught]\n");
                break;
            }
        }
    };
    m_pThread = new std::thread(fThread);
}

// debugserver
DebugServer::DebugServer()
{

}

DebugServer::~DebugServer()
{
    UnInit();
}

int DebugServer::UnInit()
{
    if (m_pThread)
    {   
        delete m_pThread;
        m_pThread = nullptr;
    }

    if (m_nSocketClient > 0)
    {
        closesocket(m_nSocketClient);
        m_nSocketClient = INVALID_SOCKET;
    }

    if (m_nSocketLisent > 0)
    {
        closesocket(m_nSocketLisent);
        m_nSocketLisent = INVALID_SOCKET;
		//终止 DLL 的使用  
		WSACleanup();
    }
}

int DebugServer::StartServer(short port)
{
    int nRet = 0;
    if (port > 1024 && port < 65534)
    {
        m_nPort = port;
        nRet = 1;
    }
    return nRet;
}

int DebugServer::StoprServer()
{
    int nRet = 0;

    return nRet;
}

int DebugServer::Send(std::string msg)
{
    int nRet = 0;

    return nRet;
}


int DebugServer::Recv(std::string& msg)
{
    int nRet = 0;

    return nRet;
}

int DebugServer::InitServer()
{
    int nRet = 0;

    // 先清理，保证资源不泄露
    UnInit();
    
    //初始化 DLL  
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

    m_nSocketLisent = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_nSocketClient >= 0)
    {   
        sockaddr_in addrServer;
        memset(&addrServer, 0, sizeof(addrServer));
        addrServer.sin_family = AF_INET;
        addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
        addrServer.sin_port = htons(m_nPort);
        if (bind(m_nSocketLisent, (sockaddr *)&addrServer, sizeof(addrServer)) >= 0)
        {
            if (listen(m_nSocketLisent, _TCP_MAX_CONN_WAIT) >= 0)
            {
                AcceptThread();
                nRet = 1;
            }
        }
    }
    return nRet;
}

int DebugServer::DisConnect()
{
    int nRet = 0;

    return nRet;
}

// lua interface
DebugServerWrapper::DebugServerWrapper(lua_State* L)
:m_Lua(L)
{

}

DebugServerWrapper::~DebugServerWrapper()
{
    m_Lua = nullptr;
}

int DebugServerWrapper::StartServer(lua_State* L)
{
    int nRet = 0;
    int port = luaL_checkinteger(L, 1);
    if (port > 1024 && port < 65534)
    {
        nRet = m_DbgSever.StartServer(port);
    }
    lua_pushinteger(L, nRet);
    return nRet;
}

int DebugServerWrapper::StoprServer(lua_State* L)
{
    int nRet = 0;
    nRet = m_DbgSever.StoprServer();
    lua_pushinteger(L, nRet);
    return 1;
}

int DebugServerWrapper::Send(lua_State* L)
{
    int nRet = 0;
    const char* msg = luaL_checkstring(L, 1);
    if (msg)
    {
        nRet = m_DbgSever.Send(std::string(msg));
    }
    lua_pushinteger(L, nRet);
    return 1;
}

int DebugServerWrapper::Recv(lua_State* L)
{
    int nRet = 0;
    std::string msg;
    int len = m_DbgSever.Recv(msg);
    if (len > 0)
    {
        lua_pushstring(L, msg.c_str());
        nRet++;
    }
    lua_pushinteger(L, nRet);
    return 1;
}

RegType DebugServerWrapper::Functions[] = 
{
    {"StartServer", &DebugServerWrapper::StartServer},
    {"StoprServer", &DebugServerWrapper::StoprServer},
    {"Send", &DebugServerWrapper::Send},
    {"Revc", &DebugServerWrapper::Recv},
    {NULL, NULL}
};

void LuaPort::Register(lua_State *L)
{
    lua_pushcfunction(L, LuaPort::Constructor);
    lua_setglobal(L,  "DebugServer");
    luaL_newmetatable(L, "DebugServer");
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, &LuaPort::GCObj);
    lua_settable(L, -3);
    // ----- 不一样的
    // 把方法也注册进userdata的元表里
    for (int i = 0; DebugServerWrapper::Functions[i].name; ++i)
    {
        lua_pushstring(L, DebugServerWrapper::Functions[i].name);
        lua_pushnumber(L, i);
        lua_pushcclosure(L, &LuaPort::Porxy, 1);
        lua_settable(L, -3);
    }
    // 注册__index方法
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
}

int LuaPort::Constructor(lua_State *L)
{
    DebugServerWrapper *obj = new DebugServerWrapper(L);
    DebugServerWrapper **a = static_cast<DebugServerWrapper **>(lua_newuserdata(L, sizeof(DebugServerWrapper *)));
    *a = obj;
    luaL_getmetatable(L, "DebugServer");
    lua_setmetatable(L, -2);
    return 1;
}

int LuaPort::Porxy(lua_State *L)
{
    int i = (int)lua_tonumber(L, lua_upvalueindex(1));
    DebugServerWrapper **obj = static_cast<DebugServerWrapper **>(luaL_checkudata(L, 1, "DebugServer"));
    return ((*obj)->*(DebugServerWrapper::Functions[i].mfunc))(L);
}

int LuaPort::GCObj(lua_State *L)
{
    DebugServerWrapper **obj = static_cast<DebugServerWrapper **>(luaL_checkudata(L, 1, "DebugServer"));
    delete (*obj);
    return 0;
}




