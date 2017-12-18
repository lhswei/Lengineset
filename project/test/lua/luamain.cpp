
#include "luluadef.h"
#include <stdlib.h>
#include <stdio.h>

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

    int nRetCode = luaL_dofile(luaEnv, "./test.lua");
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

    lua_close(luaEnv);

    // getchar();
    return 0;
}





