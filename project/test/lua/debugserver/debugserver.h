#ifndef __DEBUG_SERVER_H__
#define __DEBUG_SERVER_H__

#include <stdio.h>
#include <stdlib.h>  
#include <stddef.h>  
#include <errno.h>
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock_guard, std::adopt_lock
#include <chrono>
#include <functional>
#include <atomic>
#include <winsock2.h> 
#include <windows.h>
#include <synchapi.h>
#include <string>
#include "luawrapper.h"


#pragma comment(lib, "WS2_32")

#if defined(LDEBUG_BUILD_AS_DLL)	/* { */

#if defined(LDEBUG_CORE) || defined(LDEUBG_LIB)	/* { */
#define LDEBUG_API __declspec(dllexport)
#else						/* }{ */
#define LDEBUG_API __declspec(dllimport)
#endif						/* } */

#else				/* }{ */

#define LDEBUG_API		extern

#endif				/* } */

#define _TCP_MAX_CONN_WAIT          10
#define _TCP_CHAR_IP_LEN            33
#define _TCP_MAX_BUFFER_SIZE        (1024 * 4)

struct lua_State;
class DebugServerWrapper;

enum class DBG_RUN_STATE {DBG_NONE, DBG_DETACH, DGB_ATTACH};

struct RegType
{
    const char *name;
    int (DebugServerWrapper::*mfunc)(lua_State *L);
};

class DebugServer 
{
public:
    DebugServer();
    ~DebugServer();

public:
    int StartServer(short port);
    int StoprServer();
    int Send(std::string msg);
    int Recv(std::string& msg);
    int Dettach();
    void SetRunState(DBG_RUN_STATE st);
    bool CheckRunState(DBG_RUN_STATE st);
private:
    int InitServer();
    void AcceptThread();
    int UnInit();
    void StopThread();
private:
    int m_nSocketLisent = INVALID_SOCKET;
    int m_nSocketClient = INVALID_SOCKET;
    short m_nPort = -1;
    std::mutex m_mtx;
    std::thread* m_pThread;
    DBG_RUN_STATE m_dbgstate;
};


class DebugServerWrapper
{
public:
    DebugServerWrapper(lua_State* L);
    ~DebugServerWrapper();

public:
    int StartServer(lua_State* L);
    int StoprServer(lua_State* L);
    int Send(lua_State* L);
    int Recv(lua_State* L);

public:
    static RegType Functions[];
private:
    DebugServer m_DbgSever;
    lua_State* m_Lua = nullptr;
};

class LuaPort
{
public:
    static void Register(lua_State* L);
    static int Constructor(lua_State *L);
    static int Porxy(lua_State *L);
    static int GCObj(lua_State *L);
};

class MyTest
{
public:
	MyTest() {}
	~MyTest() {}

public:
	int Foo(int a)
	{
		printf("_Foo_(%d)\n", a);
		return 1;
	}
private:
	int id = 0;
};

class MyTestWrapper : luacpp::LunaBase<MyTest>
{
public:
	MyTestWrapper(lua_State* L):luacpp::LunaBase<MyTest>(L)
	{
		
	}
	~MyTestWrapper() {}

public:
	int LuaFoo(lua_State* L);

public:
	static luacpp::_RegType<MyTestWrapper> Functions[];
};

#endif //__DEBUG_SERVER_H__
