#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "luajit.h"

#include "modes.h"
#include "state.h"

static int lua_function_ref = LUA_NOREF;

// static int set_function(lua_State *L) {
//     if (!lua_isfunction(L, 1)) {
//         lua_pushstring(L, "Argument must be a function");
//         lua_error(L);
//     }   

//     lua_function_ref = luaL_ref(L, LUA_REGISTRYINDEX);
//     return 0; // Number of return values
// }

// lua_rawgeti(L, LUA_REGISTRYINDEX, lua_function_ref);
// if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
//     fprintf(stderr, "Error calling stored Lua function: %s\n", lua_tostring(L, -1));
//     lua_pop(L, 1); // Remove error message from stack
// }

static int l_register_mode(lua_State *L) {
    Mode* mode = parse_mode();
}


// Function to register the C functions in Lua
int luaopen_mylib(lua_State *L) {
    static const luaL_Reg mylib[] = {
        {"set_function", set_function},
        {"add_mode", add_mode},
        {NULL, NULL} // Sentinel
    };
    luaL_register(L, "lofi", mylib);
    return 1;
}