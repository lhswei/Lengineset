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
    if (!CheckRunState(DBG_RUN_STATE::DBG_NONE))
        return;

    if (m_nSocketClient > 0)
    {
        ::closesocket(m_nSocketClient);
        m_nSocketClient = INVALID_SOCKET;
    }

    StopThread();

    auto fThread = [this]() -> void {
		this->SetRunState(DBG_RUN_STATE::DBG_DETACH);
        while (this->m_nSocketLisent >= 0)
        {
            sockaddr_in sockaddrClient;
            int nRetCode = 0;
            int nsin_size = sizeof(struct sockaddr_in);
            int conn = ::accept(this->m_nSocketLisent, (struct sockaddr *)&sockaddrClient, &nsin_size);
            if (conn >= 0)
            {
				if (this->m_nSocketClient < 0)
				{
					this->m_nSocketClient = conn;
					this->m_nPing = 30;
					this->SetRunState(DBG_RUN_STATE::DBG_ATTACH);
					unsigned long ul = 1;
					::ioctlsocket(this->m_nSocketClient, FIONBIO, &ul);
					printf("debug> attach success.\n");
				}
				else
				{
					std::string msg("connection already exist.");
					::send(conn, msg.c_str(), msg.length(), 0);
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					::closesocket(conn);
					conn = INVALID_SOCKET;
				}
            }

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };
    m_pThread = new std::thread(fThread);
}



// debugserver
DebugServer::DebugServer()
:m_dbgstate(DBG_RUN_STATE::DBG_NONE),
m_pThread(nullptr),
m_ThreadConsole(),
m_pThreadRecv(nullptr)
{

}

DebugServer::~DebugServer()
{
	StopRecv();
    UnInit();
	StopConsole();
}

int DebugServer::UnInit()
{
    StopThread();
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

	SetRunState(DBG_RUN_STATE::DBG_NONE);

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
	UnInit();
    return nRet;
}

void DebugServer::StopThread()
{
    if (m_pThread)
    {   
        delete m_pThread;
        m_pThread = nullptr;
    }
}

void DebugServer::StartConsole()
{
	if (m_ThreadConsole.joinable())
		return;

	auto fThread = [this]() -> void {
		char szMessage[1024] = {0};
		do
		{
			//select();
			memset(szMessage, 0, 1024);
			if (fgets(szMessage, 1024 - 1, stdin) && this->m_qConsole.size() < 10)
			{
				if (strcmp("stop\n", szMessage) == 0)
					break;

				std::lock_guard<std::mutex> lck(this->m_mtxconsole);
				this->m_qConsole.push(szMessage);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		} while (true);
	};

	m_ThreadConsole = std::move(std::thread(fThread));
}

void DebugServer::StopConsole()
{
	if (m_ThreadConsole.joinable())
	{
		m_ThreadConsole.join();
	}

}

void DebugServer::StartRecv()
{
	if (m_pThreadRecv)
		return;

	auto fThread = [this]() -> void {
		std::string sping("ping");
		std::string spong("pong");
		int len = spong.length();
		int nstep = 0;
		do
		{
			if (!(this->CheckRunState(DBG_RUN_STATE::DBG_ATTACH)))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}

			std::string msg;
			do
			{
				msg.clear();
				this->_Recv(msg);
				if (msg.length() == len && msg == spong)
				{
					// 收到ping
					this->m_nPing = 10;
				}
				else if (msg.length() > 0)
				{
					std::lock_guard<std::mutex> lck(this->m_mtxRecv);
					if (this->m_qRecv.size() < 100)
					{
						this->m_qRecv.push(msg);
					}
				}
			} while (msg.length() > 0);

			if ( (nstep++ % 10) == 0 )
			{
				
				if (this->m_nPing-- < 0)
					this->Dettach();	// 失去链接
				else
					this->Send(sping);  // ping
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			

		} while (true);
	};

	m_pThreadRecv = new std::thread(fThread);
}

void DebugServer::StopRecv()
{
	if (m_pThreadRecv)
	{
		delete m_pThreadRecv;
		m_pThreadRecv = nullptr;
	}
}

int DebugServer::Send(std::string& msg)
{
    int nRet = 0;

    if (m_nSocketClient < 0)
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
	do
	{
		std::lock_guard<std::mutex> lck(m_mtxRecv);
		if (m_qRecv.size() > 0)
		{
			msg.swap(m_qRecv.front());
			m_qRecv.pop();
			nRet = msg.length();
		}
	} while (false);
	return nRet;
}

int DebugServer::_Recv(std::string& msg)
{
    int nRet = 0;
    if (m_nSocketClient < 0)
    {
        return 0;
    }

    char chbuffer[2048] = {0};
	int cnt = ::recv(m_nSocketClient, chbuffer, 2047, 0);
	if (cnt > 0)
	{
		//正常处理数据
		msg.append(chbuffer);
		nRet = 1;
	}
	else
	{
		if ((cnt < 0) && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
		{
			//这几种错误码，认为连接是正常的，继续接收
		}
		else if (cnt == 0)
		{
			// 断开连接，接受线程继续等待连接
			Dettach();
		}
	}
    return nRet;
}

int DebugServer::InitServer()
{
    int nRet = 0;

	StartRecv();

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
    if (m_nSocketClient >= 0)
    {
        ::closesocket(m_nSocketClient);
        m_nSocketClient = INVALID_SOCKET;
        SetRunState(DBG_RUN_STATE::DBG_DETACH);
		printf("debug> connection is broken.");
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
	return m_dbgstate == st;
}

int DebugServer::ReadCmd(std::string& cmd)
{
	int nRet = 0;
	std::lock_guard<std::mutex> lck(m_mtxconsole);
	if (m_qConsole.size() > 0)
	{
		cmd.swap(m_qConsole.front());
		m_qConsole.pop();
		nRet = cmd.length();
	}
	return nRet;
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
		std::string strmsg(msg);
        nRet = superclass::Send(strmsg);
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

int DebugServerWrapper::Dettach(lua_State* L)
{	
	superclass::Dettach();
	return 0;
}

int DebugServerWrapper::ReadCmd(lua_State* L)
{
	int nRet = 0;
	std::string msg;
	int len = superclass::ReadCmd(msg);
	if (len > 0)
	{
		lua_pushstring(L, msg.c_str());
		nRet++;
	}
	return nRet;
}

int DebugServerWrapper::StartConsole(lua_State* L)
{
	superclass::StartConsole();
	return 0;
}

int DebugServerWrapper::StopConsole(lua_State* L)
{
	superclass::StopConsole();
	return 0;
}

L_REG_TYPE(DebugServerWrapper) DebugServerWrapper::Functions[] =
{
    {"StartServer", &DebugServerWrapper::StartServer},
    {"StoprServer", &DebugServerWrapper::StoprServer},
    {"Send", &DebugServerWrapper::Send},
	{"Revc", &DebugServerWrapper::Recv},
	{"Dettach", &DebugServerWrapper::Dettach},
	{"ReadCmd", &DebugServerWrapper::ReadCmd },
	{"StartConsole", &DebugServerWrapper::StartConsole },
	{"StopConsole", &DebugServerWrapper::StopConsole },
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

