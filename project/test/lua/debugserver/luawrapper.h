#ifndef __LUA_WRAPPER_H__
#define __LUA_WRAPPER_H__

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#ifndef __stringify_1
// 将token转化成字符串
#define  __stringify_1(x)   #x
#endif

// 
#define L_REG_TYPE(T) luacpp::_RegType<T>

// 
#define L_INHERIT_TYPE(N) luacpp::LunaBase<N>

// 继承要包装类的宏
#define L_INHERIT_CALSS(N)  public L_INHERIT_TYPE(N)

// lua包装类声明
#define L_DECLARE_LUA_CLASS(T) \
public: \
	static const char* className; \
	static L_REG_TYPE(T) Functions[]; \
private: \
	const char* _Version() {return #T" lua wrapper 0.1"; }


// lua包装类定义导出的类名
// T包装类， N 导出的 c++ 类
#define L_DEFINE_LUA_CLASS(T, N) \
const char* T::className = __stringify_1(N)


// 注册
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
	 	//注册函数，在lua中创建function类型的值来表示Luna<T>::constructor
		lua_pushcfunction(L, &Luna::Constructor);

		//该函数并将类型为fucntion的值入栈
		//将上面的function值赋给全局变量className
		lua_setglobal(L, T::className);

		//创建一个新表（将用作metatable），将新表放到栈顶并建立表和registry中类型名的联系。
		//这个关联是双向的：使用类型名作为表的key；
		//同时使用表作为类型名的key（这种双向的关联，使得其他的两个函数的实现效率更高）。
		luaL_newmetatable(L, T::className);
		/*
		Lua以__gc元方法的方式提供了finalizers。这个元方法只对userdata类型的值有效。
		当一个userdatum将被收集的时候，并且usedatum有一个__gc域，
		Lua会调用这个域的值（应该是一个函数）：以userdatum作为这个函数的参数调用。
		这个函数负责释放与userdatum相关的所有资源。
		*/
		lua_pushstring(L, "__gc");
		lua_pushcfunction(L, &Luna::GCObj);
		lua_settable(L, -3);

		// 把方法也注册进userdata的元表里
		for (int i = 0; T::Functions[i].name; ++i)
		{
			lua_pushstring(L, T::Functions[i].name);
			lua_pushnumber(L, i);
			lua_pushcclosure(L, &Luna::Porxy, 1);
			lua_settable(L, -3);
		}

		// 注册__index方法
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
		//函数获取registry中的tname对应的metatable。
		luaL_getmetatable(L, T::className);
		//lua_setmetatable函数将表(get到的)出栈，并将其设置为给定位置(上面userdatum)对象的metatable。
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
