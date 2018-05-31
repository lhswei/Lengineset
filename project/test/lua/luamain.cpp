
#include "./debugserver/debugserver.h"
#include "luluadef.h"
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>  
#include "./debugserver/luawrapper.h"
int g_Add(lua_State *L)
{
    lua_Number a = luaL_checknumber(L, 1);
    lua_Number b = luaL_checknumber(L, 2);
    lua_Number nSum = a + b;
    lua_pushnumber(L, nSum);
    return 1;
}

int main(int argc, char *argv[])
{
    lua_State* luaEnv = luaL_newstate();
    if (!luaEnv)
    {
        printf("lua_open fail!\n");
        return 0;
    }

    luaL_openlibs(luaEnv);

    struct luaL_Reg lib[] =
    {
        {"add", g_Add},
        {NULL, NULL}
    };

    luaL_register(luaEnv, "glib", lib);
    LuaPort::Register(luaEnv);
	luacpp::Luna<MyTestWrapper>::Register(luaEnv);
	char *buffer;
	char paht[256] = { 0 };
	//也可以将buffer作为输出参数  
	if ((buffer = getcwd(NULL, 0)) == NULL)
	{
		perror("getcwd error");
	}
	else
	{
		//sprintf(paht, "%s\\test.lua", buffer);
		sprintf(paht, "%s\\test.lua", "e:\\github\\Lengineset\\project\\test\\lua\\cmake_build\\Debug");
		printf("%s\n", buffer);
		free(buffer);
	}

    int nRetCode = luaL_dofile(luaEnv, paht);
    if(nRetCode)
    {
        printf("error pcall:\n %s\n", luaL_tolstring(luaEnv, -1, NULL));


    }

    lua_getglobal(luaEnv, "subtest");
    lua_pushnumber(luaEnv, 8);
    lua_pushnumber(luaEnv, 5);

    lua_pcall(luaEnv, 2, 1, 0);

    int nRetCall = 0;
    if (lua_isnumber(luaEnv, -1))
    {
        nRetCall = lua_tonumber(luaEnv, -1);
        printf("pcall: 8 - 5 = %d\n", nRetCall);
    }
    lua_pop(luaEnv, -1);
    while (true)
    {
        lua_getglobal(luaEnv, "on_tick");
        lua_pcall(luaEnv, 0, 0, 0);
        lua_pop(luaEnv, -1);
        // printf("waitting for message...\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    lua_close(luaEnv);

    getchar();
    return 0;
}





