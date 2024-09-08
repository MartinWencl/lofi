#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "luajit.h"

struct Mode {
    int f_first;
    int f_update;
};

static struct Mode lua_new_mode;
static int lua_function_ref = LUA_NOREF;

static int set_function(lua_State *L) {
    if (!lua_isfunction(L, 1)) {
        lua_pushstring(L, "Argument must be a function");
        lua_error(L);
    }   

    lua_function_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    return 0; // Number of return values
}

static int add_mode(lua_State *L) {
    if (!lua_isfunction(L, 1) || !lua_isfunction(L, 2)) {
        lua_pushstring(L, "The arguments must be functions");
        lua_error(L);
    }   
   
    lua_new_mode.f_update = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_new_mode.f_first = luaL_ref(L, LUA_REGISTRYINDEX);
    return 0; // Number of return values
}

void call_stored_lua_function(lua_State *L) {
    if (lua_function_ref != LUA_NOREF) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, lua_function_ref);
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            fprintf(stderr, "Error calling stored Lua function: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1); // Remove error message from stack
        }

        lua_rawgeti(L, LUA_REGISTRYINDEX, lua_new_mode.f_first);
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            fprintf(stderr, "Error calling stored Lua function: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1); // Remove error message from stack
        }

        lua_rawgeti(L, LUA_REGISTRYINDEX, lua_new_mode.f_update);
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            fprintf(stderr, "Error calling stored Lua function: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1); // Remove error message from stack
        }
    } else {
        fprintf(stderr, "No Lua function stored\n");
    }
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
