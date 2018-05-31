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
	const char* _Version() {return #T" lua wrapper 0.1"; }


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
		luaL_getmetatable(L, T::className);
		//lua_setmetatable��������(get����)��ջ������������Ϊ����λ��(����userdatum)�����metatable��
		lua_setmetatable(L, -2);


		return 1;
	}
    static int Porxy(lua_State *L)
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
