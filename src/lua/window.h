#ifndef LUA_WINDOW_H
#define LUA_WINDOW_H

#include "lua.h"
#include "ui/window.h"

void ExportWindowConfigToLua(lua_State *L, WindowConfig config);
void LoadWindowConfigFromLua(lua_State *L, WindowConfig *config);

#endif // LUA_WINDOW_H
