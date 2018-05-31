// 
// @filename:    luawrpper.h
// @Author:      luhengsi
// @DateTime:    2018-05-31 11:50:23
// @Description: define lua wrapper
// @Description: 

/*
һ�� c++ �ർ���� lua �ļ򵥵Ļ���ģ��
ʹ�÷�����
	1. �����Ͷ��屻������c++��: MyTest�����������ǲ���lua�������ࡣ
	   Ҳ������lua�����������󶨡�
	2. �����Ͷ��屣��װ��: MyTestWarpper�������Ͷ�����Ҫ�õ�����һϵ�еĺ궨�壬
	   ������ MyTestWarpper �� MyTest�Ĺ�ϵ��
	3. ʹ�� L_LUA_WRAPPER_REGISTER(MyTestWarpper, luaEvn);������ע��Ҫ�������࣬
	   �Լ���ķ�����

�ļ��ṹ��
--=========================================================================
-----------test.h------------
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

	// ��Ҫ���������궨��public��Ա�������м��ı�������Ա�ķ���Ȩ��
	L_DECLARE_LUA_CLASS(MyTestWrapper);
};

--=========================================================================
-------------test.cpp--------------

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
--========================================================================
-------------main.cpp----------------
int main(int argc, char *argv[])
{
	lua_State* luaEnv = luaL_newstate();
	if (!luaEnv)
	{
		printf("lua_open fail!\n");
		return 0;
	}

	luaL_openlibs(luaEnv);
	L_LUA_WRAPPER_REGISTER(MyTestWrapper, luaEnv);
	int nRetCode = luaL_dofile(luaEnv, "test.lua");
	if(nRetCode)
	{
		printf("error pcall:\n %s\n", luaL_tolstring(luaEnv, -1, NULL));
	}
	lua_close(luaEnv);
	getchar();
	return 0;
}
--========================================================================
---------------test.lua--------------------------------
local mytest = MyTest()
mytest:Foo(99);
--========================================================================

*/

#ifndef __LUA_WRAPPER_H__
#define __LUA_WRAPPER_H__

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#ifndef __stringify_1
// ��tokenת�����ַ���
#define  __stringify_1(x)   #x
#endif

// 
#define L_REG_TYPE(T) luacpp::_RegType<T>

// 
#define L_INHERIT_TYPE(N) luacpp::LunaBase<N>

// �̳�Ҫ��װ��ĺ�
#define L_INHERIT_CALSS(N)  public L_INHERIT_TYPE(N)

// lua��װ������
#define L_DECLARE_LUA_CLASS(T) \
public: \
	static const char* className; \
	static L_REG_TYPE(T) Functions[]; \
private: \
	const char* _Version() { return #T" lua wrapper 0.1"; }


// lua��װ�ඨ�嵼��������
// T��װ�࣬ N ������ c++ ��
#define L_DEFINE_LUA_CLASS(T, N) \
const char* T::className = __stringify_1(N)


// ע��
#define L_LUA_WRAPPER_REGISTER(T, L) luacpp::Luna<T>::Register(L) 

namespace luacpp
{

template <typename T>
struct _RegType
{
	const char *name;
	int (T::*mfunc)(lua_State *L);
};

template <typename N>
class LunaBase : public N
{
public:
	LunaBase(lua_State* L) :m_Lua(L)
	{

	}

	~LunaBase()
	{
		m_Lua = nullptr;
	}

protected:
	lua_State * m_Lua = nullptr;
};

template <typename T>
class Luna 
{
public:

    static void Register(lua_State* L)
	{
	 	//ע�ắ������lua�д���function���͵�ֵ����ʾLuna<T>::constructor
		lua_pushcfunction(L, &Luna::Constructor);

		//�ú�����������Ϊfucntion��ֵ��ջ
		//�������functionֵ����ȫ�ֱ���className
		lua_setglobal(L, T::className);

		//����һ���±�������metatable�������±�ŵ�ջ�����������registry������������ϵ��
		//���������˫��ģ�ʹ����������Ϊ���key��
		//ͬʱʹ�ñ���Ϊ��������key������˫��Ĺ�����ʹ������������������ʵ��Ч�ʸ��ߣ���
		luaL_newmetatable(L, T::className);
		/*
		Lua��__gcԪ�����ķ�ʽ�ṩ��finalizers�����Ԫ����ֻ��userdata���͵�ֵ��Ч��
		��һ��userdatum�����ռ���ʱ�򣬲���usedatum��һ��__gc��
		Lua�����������ֵ��Ӧ����һ������������userdatum��Ϊ��������Ĳ������á�
		������������ͷ���userdatum��ص�������Դ��
		*/
		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, &Luna::GCObj);
		lua_settable(L, -3);

		// �ѷ���Ҳע���userdata��Ԫ����
		for (int i = 0; T::Functions[i].name; ++i)
		{
			lua_pushstring(L, T::Functions[i].name);
			lua_pushnumber(L, i);
			lua_pushcclosure(L, &Luna::Proxy, 1);
			lua_settable(L, -3);
		}

		// ע��__index����
		lua_pushstring(L, "__index");
		lua_pushvalue(L, -2);
		lua_settable(L, -3);
	}

    static int Constructor(lua_State *L)
	{
		T* obj = new T(L);

		lua_newtable(L);

		lua_pushnumber(L, 0);
		//void *lua_newuserdata (lua_State *L, size_t size);
		T** a =  static_cast<T **>(lua_newuserdata(L, sizeof(T*)));
		
		*a = obj;
		//void  luaL_getmetatable (lua_State *L, const char *tname);
		//������ȡregistry�е�tname��Ӧ��metatable��
		luaL_getmetatable(L, T::className);
		//lua_setmetatable��������(get����)��ջ������������Ϊ����λ��(����userdatum)�����metatable��
		lua_setmetatable(L, -2);


		return 1;
	}
    static int Proxy(lua_State *L)
	{
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		T **obj = static_cast<T **>(luaL_checkudata(L, 1, T::className));
		lua_remove(L, 1);
		return ((*obj)->*(T::Functions[i].mfunc))(L);
	}

    static int GCObj(lua_State *L)
	{
		T **obj = static_cast<T **>(luaL_checkudata(L, 1, T::className));
		delete (*obj);
		return 0;
	}
};

}

#endif // __LUA_WRAPPER_H__
