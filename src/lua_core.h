#ifndef LUA_CORE_H
#define LUA_CORE_H

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

// #include "callbacks/events.h"
#include "state.h"
#include "mode_manager.h"

#ifndef lua_rawlen
#define lua_rawlen(L, index) lua_objlen(L, (index))
#endif

#define MAX_LIST_ITEMS 1024

#define LUA_OK 0


lua_State* NewLuaState(State* state);

void LoadLua(State* state, lua_State* L);
void LoadOptionsFromLua(State* state, lua_State* L);

int DispatchLuaEvent(lua_State* L, State* state, Mode* mode, EventType event);
void InitModeManager(ModeManager* modeManager);

void PushStateToLua(lua_State* L, State* state, Mode* mode);
void UpdateStateFromLua(lua_State* L, State* state, Mode* mode);

#endif // LUA_CORE_H
