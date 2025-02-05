#ifndef LUA_FONT_H
#define LUA_FONT_H

#include "font.h"
#define DEFAULT_FONT_SIZE 20

#include "lua.h"
#include "ui/state.h"

void ExportFontConfigToLua(lua_State *L, FontConfig config);
void LoadFontConfigFromLua(lua_State* L, UIState* ui);

#endif // LUA_FONT_H
