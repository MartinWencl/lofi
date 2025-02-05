#ifndef LUA_CORE_H
#define LUA_CORE_H

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

// #include "callbacks/events.h"
#include "state.h"
#include "modes.h"

#ifndef lua_rawlen
#define lua_rawlen(L, index) lua_objlen(L, (index))
#endif

#define MAX_LIST_ITEMS 1024

#define LUA_OK 0


int DispatchLuaEvent(State *state, ModeManager *modeManager, EventType event);
void InitLua(const char* initPath, lua_State* L);
void InitModeManager(ModeManager *modeManager);

#endif // LUA_CORE_H
