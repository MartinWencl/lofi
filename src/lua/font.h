#ifndef LUA_FONT_H
#define LUA_FONT_H

//TODO: Change for DPI/monitor scaling
#define DEFAULT_FONT_SIZE 20

#include "lua.h"
#include "ui/state.h"

void LoadFontConfigFromLua(lua_State* L, UIState* ui);

#endif // LUA_FONT_H
