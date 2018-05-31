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


class DebugServerWrapper: L_INHERIT_CALSS(DebugServer)
{
public:
	typedef DebugServer superclass;
    DebugServerWrapper(lua_State* L);
    ~DebugServerWrapper();

public:
    int StartServer(lua_State* L);
    int StoprServer(lua_State* L);
    int Send(lua_State* L);
    int Recv(lua_State* L);

	// 需要放最后，这个宏定义public成员，若放中间会改变其他成员的访问权限
	L_DECLARE_LUA_CLASS(DebugServerWrapper);
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
		id = a;
		return 1;
	}
private:
	int id = 0;
};

class MyTestWrapper : L_INHERIT_CALSS(MyTest)
{
public:
	MyTestWrapper(lua_State* L):L_INHERIT_TYPE(MyTest)(L)
	{
		
	}
	~MyTestWrapper() {}

public:
	int LuaFoo(lua_State* L);

	// 需要放最后，这个宏定义public成员，若放中间会改变其他成员的访问权限
	L_DECLARE_LUA_CLASS(MyTestWrapper);
};

#endif //__DEBUG_SERVER_H__
