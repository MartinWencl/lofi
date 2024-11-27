#ifndef LUA_CORE_H
#define LUA_CORE_H

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "events.h"
#include "core.h"
#include "modes.h"

#ifndef lua_rawlen
#define lua_rawlen(L, index) lua_objlen(L, (index))
#endif

#define MAX_LIST_ITEMS 1024

#define LUA_OK 0


int DispatchLuaEvent(AppState *state, ModeManager *modeManager, AppEventType event);
lua_State* InitLua(const char* initPath, ModeManager *modeManager);
void InitModeManager(ModeManager *modeManager);

#endif // LUA_CORE_H