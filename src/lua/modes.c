#include "lua/modes.h"

#include "raylib.h"
#include "mode_manager.h"

#include "lauxlib.h"
#include "lualib.h"

#include <string.h>

static ModeManager* GetModeManager(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, MODE_MANAGER_KEY);
    ModeManager* modeManager = (ModeManager*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    return modeManager;
}

int lofi_RegisterMode(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    
    ModeManager *modeManager = GetModeManager(L);
    if (modeManager->modeCount >= MAX_MODES) {
        return luaL_error(L, "Maximum number of modes reached");
    }

    Mode* mode = &modeManager->modes[modeManager->modeCount++];
    InitMode(mode);  // Initialize the new mode
    mode->id = modeManager->modeCount - 1;
    
    // Get name
    lua_getfield(L, 1, "name");
    if (!lua_isstring(L, -1)) {
        lua_pop(L, 1);
        return luaL_error(L, "name must be a string");
    }
    strncpy(mode->name, lua_tostring(L, -1), MAX_NAME_LENGTH - 1);
    mode->name[MAX_NAME_LENGTH - 1] = '\0';
    lua_pop(L, 1);
    
    // Get prefix
    lua_getfield(L, 1, "prefix");
    if (!lua_isstring(L, -1)) {
        lua_pop(L, 1);
        return luaL_error(L, "prefix must be a string");
    }
    strncpy(mode->prefix, lua_tostring(L, -1), MAX_PREFIX_LENGTH - 1);
    mode->prefix[MAX_PREFIX_LENGTH - 1] = '\0';
    lua_pop(L, 1);
    
    // Register callbacks if present
    lua_getfield(L, 1, "callbacks");
    if (!lua_isnil(L, -1)) {
        if (!lua_istable(L, -1)) {
            lua_pop(L, 1);
            return luaL_error(L, "callbacks must be a table");
        }
        
        // Iterate through the callbacks table
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            // Key should be the event name (string)
            if (!lua_isstring(L, -2)) {
                lua_pop(L, 2);
                continue;
            }
            
            const char* eventName = lua_tostring(L, -2);
            TraceLog(LOG_DEBUG, "Registering callback %s", eventName);
            
            // Value should be a function
            if (lua_isfunction(L, -1)) {
                // Create reference for the function
                int funcRef = luaL_ref(L, LUA_REGISTRYINDEX);
                if (!AddCallback(L, mode, eventName, funcRef)) {
                    luaL_unref(L, LUA_REGISTRYINDEX, funcRef);
                    TraceLog(LOG_WARNING, "Failed to add callback for event: %s", eventName);
                }
            } else {
                lua_pop(L, 1);
            }
        }
    }
    lua_pop(L, 1);
    
    TraceLog(LOG_DEBUG, "LUA MODES: Mode registration complete.");
    return 0;
}
