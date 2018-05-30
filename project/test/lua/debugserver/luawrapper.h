#ifndef __LUA_WRAPPER_H__
#define __LUA_WRAPPER_H__

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#ifndef __stringify_1
#define  __stringify_1(x)   #x
#endif

namespace luacpp
{

template <typename T>
struct _RegType
{
	const char *name;
	int (T::*mfunc)(lua_State *L);
};

template <typename T>
class LunaBase : public T
{
public:
	LunaBase(lua_State* L) :m_Lua(L)
	{

	}

	~LunaBase()
	{
		m_Lua = nullptr;
	}

public:
	lua_State * m_Lua = nullptr;
};

template <typename T, typename N>
class Luna 
{
public:

    static void Register(lua_State* L)
	{
	 	//ע�ắ������lua�д���function���͵�ֵ����ʾLuna<T>::constructor
		lua_pushcfunction(L, &Luna::Constructor);

		//�ú�����������Ϊfucntion��ֵ��ջ
		//�������functionֵ����ȫ�ֱ���className
		lua_setglobal(L, __stringify_1(N));

		//����һ���±�������metatable�������±�ŵ�ջ�����������registry������������ϵ��
		//���������˫��ģ�ʹ����������Ϊ���key��
		//ͬʱʹ�ñ���Ϊ��������key������˫��Ĺ�����ʹ������������������ʵ��Ч�ʸ��ߣ���
		luaL_newmetatable(L,  __stringify_1(N));
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
			lua_pushcclosure(L, &Luna::Porxy, 1);
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
		luaL_getmetatable(L, __stringify_1(N));
		//lua_setmetatable��������(get����)��ջ������������Ϊ����λ��(����userdatum)�����metatable��
		lua_setmetatable(L, -2);


		return 1;
	}
    static int Porxy(lua_State *L)
	{
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		T **obj = static_cast<T **>(luaL_checkudata(L, 1, __stringify_1(N)));
		lua_remove(L, 1);
		return ((*obj)->*(T::Functions[i].mfunc))(L);
	}

    static int GCObj(lua_State *L)
	{
		T **obj = static_cast<T **>(luaL_checkudata(L, 1, __stringify_1(N)));
		delete (*obj);
		return 0;
	}
};

}

#endif // __LUA_WRAPPER_H__
