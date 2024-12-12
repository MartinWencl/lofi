#include "lua/window.h"
#include "raylib.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

void ExportWindowConfigToLua(lua_State *L, WindowConfig config) {
    TraceLog(LOG_DEBUG, "start of export");
    lua_getglobal(L, "lofi");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_setglobal(L, "lofi");
    }

    lua_getfield(L, -1, "opt");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_setfield(L, -2, "opt");
    }

    lua_newtable(L); 

    lua_pushstring(L, "refresh_rate");
    lua_pushinteger(L, config.refreshRate);
    lua_settable(L, -3);

    lua_pushstring(L, "window_percent_width");
    lua_pushnumber(L, config.windowPercentWidth);
    lua_settable(L, -3);

    lua_pushstring(L, "window_percent_height");
    lua_pushnumber(L, config.windowPercentHeight);
    lua_settable(L, -3);

    lua_pushstring(L, "opacity");
    lua_pushnumber(L, config.opacity);
    lua_settable(L, -3);

    lua_pushstring(L, "decoration");
    lua_pushboolean(L, config.decoration);
    lua_settable(L, -3);

    // Set the `window` table in `lofi.opt`
    lua_setfield(L, -2, "window");

    lua_pop(L, 2); // Cleanup, removing `lofi` and `opt`
}

void LoadWindowConfigFromLua(lua_State *L, WindowConfig *config) {
    lua_getglobal(L, "lofi");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1); // `lofi` table not found
        TraceLog(LOG_ERROR, "Cant load window config: table lofi not found!");
        return;
    }

    lua_getfield(L, -1, "opt");
    if (lua_isnil(L, -1)) {
        lua_pop(L, 2); // Cleanup `lofi` and `opt`
        TraceLog(LOG_ERROR, "Cant load window config: table window not found!");
        return;
    }

    lua_getfield(L, -1, "window");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 3); // Cleanup stack
        TraceLog(LOG_ERROR, "Cant load window config: table window not found!");
        config = &DEFAULT_WINDOW_CONFIG;
        return;
    }

    // At this point, `window` table is at the top of the stack
    lua_pushstring(L, "refresh_rate");
    lua_gettable(L, -2);
    config->refreshRate = lua_isnumber(L, -1) ? lua_tointeger(L, -1) :  DEFAULT_WINDOW_CONFIG.refreshRate;
    lua_pop(L, 1);

    lua_pushstring(L, "window_percent_width");
    lua_gettable(L, -2);
    config->windowPercentWidth = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : DEFAULT_WINDOW_CONFIG.windowPercentWidth;
    lua_pop(L, 1);

    lua_pushstring(L, "window_percent_height");
    lua_gettable(L, -2);
    config->windowPercentHeight = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : DEFAULT_WINDOW_CONFIG.windowPercentHeight;
    lua_pop(L, 1);

    lua_pushstring(L, "opacity");
    lua_gettable(L, -2);
    config->opacity = lua_isnumber(L, -1) ? lua_tonumber(L, -1) : DEFAULT_WINDOW_CONFIG.opacity;
    lua_pop(L, 1);

    lua_pushstring(L, "decoration");
    lua_gettable(L, -2);
    config->decoration = lua_isboolean(L, -1) ? lua_toboolean(L, -1) : DEFAULT_WINDOW_CONFIG.decoration;
    lua_pop(L, 1);

    TraceLog(LOG_INFO, "Loaded window config values from lua:");
    TraceLog(LOG_INFO, "Refresh Rate: %d", config->refreshRate);
    TraceLog(LOG_INFO, "Window Percent Width: %f", config->windowPercentWidth);
    TraceLog(LOG_INFO, "Window Percent Height: %f", config->windowPercentHeight);
    TraceLog(LOG_INFO, "Opacity: %f", config->opacity);
    TraceLog(LOG_INFO, "Decoration: %s", config->decoration ? "true" : "false");


    lua_pop(L, 3); // Cleanup `lofi`, `opt`, and `window`
}
