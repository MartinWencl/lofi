#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "luajit.h"

#include "lua"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <script.lua>\n", argv[0]);
        return EXIT_FAILURE;
    }

    lua_State *L = luaL_newstate();
    if (L == NULL) {
        fprintf(stderr, "Failed to create Lua state\n");
        return EXIT_FAILURE;
    }

    luaL_openlibs(L);
    // Register the library
    luaopen_mylib(L);
    lua_setglobal(L, "mylib");
    
    // Load and run the Lua script from the first command-line argument
    if (luaL_dofile(L, argv[1]) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); // Remove error message from stack
    }

    // Call the stored Lua function
    call_stored_lua_function(L);

    lua_close(L);
    return EXIT_SUCCESS;
}
