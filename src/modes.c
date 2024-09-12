#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "luajit.h"

#include "modes.h"
#include "state.h"

static Mode* parse_mode(lua_State* L) {
    Mode* mode = (Mode*)malloc(sizeof(Mode));
    if (!mode) {
        luaL_error(L, "Failed to allocate memory for mode");
        return NULL;
    }
    
    lua_getfield(L, -1, "name");
    mode->name = luaL_checkstring(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "description");
    mode->description = luaL_checkstring(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "on_init");
    if (lua_isfunction(L, -1)) {
        // Store a reference to the function
        mode->on_init = lua_tocfunction(L, -1);
    } else {
        mode->on_init = NULL;
    }
    lua_pop(L, 1);
    
    lua_getfield(L, -1, "on_input_change");
    if (lua_isfunction(L, -1)) {
        // Store a reference to the function
        mode->on_input_change = lua_tocfunction(L, -1);
    } else {
        mode->on_input_change = NULL;
    }
    lua_pop(L, 1);
    
    lua_getfield(L, -1, "on_item_render");
    if (lua_isfunction(L, -1)) {
        // Store a reference to the function
        mode->on_item_render = lua_tocfunction(L, -1);
    } else {
        mode->on_item_render = NULL;
    }
    lua_pop(L, 1);
    
    lua_getfield(L, -1, "on_select");
    if (lua_isfunction(L, -1)) {
        // Store a reference to the function
        mode->on_select = lua_tocfunction(L, -1);
    } else {
        mode->on_select = NULL;
    }
    lua_pop(L, 1);

    return mode;
}