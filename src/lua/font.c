#include "lua/font.h"
#include "lauxlib.h"
#include <string.h>

void LoadFontConfigFromLua(lua_State* L, UIState* ui) {
    lua_getglobal(L, "opt");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1); // Remove non-table value
        // Set defaults directly without logging error
        ui->config.font.name = "default";
        ui->config.font.size = DEFAULT_FONT_SIZE;
        return;
    }

    lua_getfield(L, -1, "font");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 2); // Cleanup stack from 'opt' and non-table
        // Set defaults directly without logging error
        ui->config.font.name = "default";
        ui->config.font.size = DEFAULT_FONT_SIZE;
        return;
    }

    lua_getfield(L, -1, "name");
    if (lua_isstring(L, -1)) {
        strncpy(ui->config.font.name, lua_tostring(L, -1), sizeof(ui->config.font.name) - 1);
        ui->config.font.name[sizeof(ui->config.font.name) - 1] = '\0';
    } else {
        ui->config.font.name = "default";
    }
    lua_pop(L, 1);

    lua_getfield(L, -1, "size");
    if (lua_isnumber(L, -1)) {
        ui->config.font.size = (int)lua_tointeger(L, -1);
    } else {
        ui->config.font.size = DEFAULT_FONT_SIZE;
    }
    lua_pop(L, 2);
}
