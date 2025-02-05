#include "lua/font.h"
#include "font.h"
#include "lua/macros.h"
#include "lauxlib.h"
#include <string.h>

void LoadFontConfigFromLua(lua_State* L, UIState* ui) {
    LUA_GET_GLOBAL_TABLE("lofi");
    LUA_GET_FIELD("opt");
    LUA_GET_FIELD("font");

    LUA_GET_STRING_FIELD(ui->config.font.name, "name", "default");
    LUA_GET_INT_FIELD(ui->config.font.size, "size", DEFAULT_FONT_SIZE);

    TraceLog(LOG_INFO, "LUA: Loaded font config values from lua:");
    TraceLog(LOG_INFO, "LUA: Loaded - Font Name: %s", ui->config.font.name);
    TraceLog(LOG_INFO, "LUA: Loaded - Font Size: %d", ui->config.font.size);

    LUA_POP(2);  // Remove opt and font tables
}

void ExportFontConfigToLua(lua_State *L, FontConfig config) {
    LUA_GET_GLOBAL_TABLE("lofi");
    LUA_GET_FIELD("opt");
    
    LUA_SET_TABLE("font",
        LUA_SET_STRING(config.name, "name");
        LUA_SET_INT(config.size, "size");
    );
    
    LUA_POP(2);  // Remove lofi and opt tables
}
