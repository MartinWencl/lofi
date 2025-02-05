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
    TraceLog(LOG_DEBUG, "LUA MODES: Retrieved mode manager from user data.");
    return modeManager;
}

int lofi_RegisterMode(lua_State* L) {
    TraceLog(LOG_DEBUG, "LUA MODES: A call to register a mode.");
    
    // Check that we got a table
    luaL_checktype(L, 1, LUA_TTABLE);
    
    ModeManager *modeManager = GetModeManager(L);
    if (modeManager->modeCount >= MAX_MODES) {
        return luaL_error(L, "Maximum number of modes reached");
    }

    // Create mode
    Mode* mode = &modeManager->modes[modeManager->modeCount++];
    mode->id = modeManager->modeCount - 1;
    
    // Get and validate name (required string)
    lua_getfield(L, 1, "name");
    if (!lua_isstring(L, -1)) {
        lua_pop(L, 1);
        return luaL_error(L, "name must be a string");
    }
    strncpy(mode->name, lua_tostring(L, -1), MAX_NAME_LENGTH - 1);
    mode->name[MAX_NAME_LENGTH - 1] = '\0';
    lua_pop(L, 1);
    
    // Get and validate prefix (required string)
    lua_getfield(L, 1, "prefix");
    if (!lua_isstring(L, -1)) {
        lua_pop(L, 1);
        return luaL_error(L, "prefix must be a string");
    }
    strncpy(mode->prefix, lua_tostring(L, -1), MAX_PREFIX_LENGTH - 1);
    mode->prefix[MAX_PREFIX_LENGTH - 1] = '\0';
    lua_pop(L, 1);
    
    TraceLog(LOG_DEBUG, "LUA MODES: Mode registration complete.");
    return 0;
}
