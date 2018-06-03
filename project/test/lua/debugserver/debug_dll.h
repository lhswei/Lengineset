#ifndef __DEBUG_DLL_H__
#define __DEBUG_DLL_H__

#include "luaconf.h"
#include "debugserver.h"


LDEBUG_API int luaopen_ldebugserver(lua_State* L);


#endif //__DEBUG_DLL_H__