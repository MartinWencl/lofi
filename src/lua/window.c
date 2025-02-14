#include "lua/window.h"
#include "lua/macros.h"
#include "raylib.h"
#include "lua.h"

void ExportWindowConfigToLua(lua_State *L, WindowConfig config) {
    LUA_GET_GLOBAL_TABLE("lofi");
    LUA_GET_FIELD("opt");
    
    LUA_SET_TABLE("window",
        LUA_SET_INT(config.refreshRate, "refresh_rate");
        LUA_SET_NUMBER(config.windowPercentWidth, "window_percent_width");
        LUA_SET_NUMBER(config.windowPercentHeight, "window_percent_height");
        LUA_SET_NUMBER(config.opacity, "opacity");
        LUA_SET_BOOL(config.decoration, "decoration");
    );
    
    LUA_POP(2);  // Remove lofi and opt tables
}

void LoadWindowConfigFromLua(lua_State *L, WindowConfig *config) {
    LUA_GET_GLOBAL_TABLE("lofi");
    LUA_GET_FIELD("opt");
    LUA_GET_FIELD("window");

    LUA_GET_NUMBER_FIELD(config->refreshRate, "refresh_rate", 
                      DEFAULT_WINDOW_CONFIG.refreshRate);
    LUA_GET_NUMBER_FIELD(config->windowPercentWidth, "window_percent_width", 
                        DEFAULT_WINDOW_CONFIG.windowPercentWidth);
    LUA_GET_NUMBER_FIELD(config->windowPercentHeight, "window_percent_height", 
                        DEFAULT_WINDOW_CONFIG.windowPercentHeight);
    LUA_GET_NUMBER_FIELD(config->opacity, "opacity", 
                        DEFAULT_WINDOW_CONFIG.opacity);
    LUA_GET_BOOL_FIELD(config->decoration, "decoration", 
                       DEFAULT_WINDOW_CONFIG.decoration);
    LUA_POP(3);  // Remove lofi, opt, and window tables
}
