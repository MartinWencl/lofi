#include "lua_core.h"
#include "events.h"
#include "font.h"
#include "modes.h"
#include "utils.h"

#include "lua.h"
#include "lualib.h"

// #include "ui/window.h"

#include "lua/macros.h"
#include "lua/window.h"
#include "lua/theme.h"
#include "lua/font.h"

#include <stdlib.h>
#include <string.h>

#define MODE_MANAGER_KEY "MODE_MANAGER"

static void PushStateToLua(lua_State* L, State* state) {
    lua_newtable(L);
    
    // Push input text
    lua_pushstring(L, state->input);
    lua_setfield(L, -2, "input_text");
    
    // Push list items
    lua_newtable(L);
    for (int i = 0; i < state->listCount; i++) {
        lua_pushstring(L, state->list[i]);
        lua_rawseti(L, -2, i + 1);
    }
    lua_setfield(L, -2, "list_tems");
    
    // Push other state fields
    lua_pushinteger(L, state->listCount);
    lua_setfield(L, -2, "list_count");
    
    lua_pushinteger(L, state->focus.index);
    lua_setfield(L, -2, "focus");
    TraceLog(LOG_DEBUG, "STATE: Pushed the state to lua.");
}

static void UpdateStateFromLua(lua_State* L, State* state) {
    // Ensure we're working with a table
    luaL_checktype(L, -1, LUA_TTABLE);
    
    // Update input text
    lua_getfield(L, -1, "input_text");
    if (!lua_isnil(L, -1)) {
        const char* input = lua_tostring(L, -1);
        strncpy(state->input, input, INPUT_TEXT_MAX_SIZE - 1);
    }
    lua_pop(L, 1);
    
    // Clear existing list items and reset the count
    ClearListViewExList(&(state->list), &(state->listCount));

    // Update list items
    lua_getfield(L, -1, "list");
    if (!lua_isnil(L, -1)) {
        int listCount = lua_rawlen(L, -1);
        
        for (int i = 1; i <= listCount; i++) {
            lua_rawgeti(L, -1, i);
            const char* item = lua_tostring(L, -1);
            AddToListViewExList(&(state->list), &(state->listCount), item);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    
    // Update focus
    lua_getfield(L, -1, "focus");
    if (!lua_isnil(L, -1)) {
        state->focus.index = lua_tointeger(L, -1);
    }
    lua_pop(L, 1);
    TraceLog(LOG_DEBUG, "STATE: Updated the state from lua.");
}

int DispatchLuaEvent(State *state, ModeManager *modeManager, EventType event) {
    TraceLog(LOG_DEBUG, "LUA EVENT: Dispatching lua event.");
    
    //TEMP: Add switching modes to lua
    // modeManager->currentMode = &(modeManager->modes[0]); if(modeManager->currentMode == NULL) {
    //     TraceLog(LOG_ERROR, "LUA EVENT: No current mode set!");
    //     return 0;
    // }
    //
    // int ref = modeManager->currentMode->callback_refs[event];
    //
    // if (ref == LUA_NOREF || !modeManager->luaState) return 0;
    //
    // lua_State* L = modeManager->luaState;
    // 
    // lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    // 
    // // Push current app state
    // PushStateToLua(L, state);
    // 
    // // Call the Lua function and check for errors
    // if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
    //     const char *errorMessage = lua_tostring(L, -1);
    //     TraceLog(LOG_ERROR, "LUA EVENT: Failed to call the corresponding lua function! Error: %s", errorMessage);
    //     lua_pop(L, 1);
    //     return 0;
    // }
    //
    // // Check if the function returned exactly one table
    // if (lua_type(L, -1) != LUA_TTABLE) {
    //     TraceLog(LOG_ERROR, "LUA EVENT: Function did not return a table!");
    //     lua_pop(L, 1); // Remove the non-table return value
    //     return 0;
    // }
    // 
    // // Update app state from returned table
    // UpdateStateFromLua(L, state);
    // 
    // // Remove the returned state table
    // lua_pop(L, 1);
    
    return 1;
}



int lofi_Log(lua_State *L) {
    // Check if exactly two arguments are provided
    if (lua_gettop(L) != 2) {
        lua_pushstring(L, "Requires two arguments: log_level and log_message");
        lua_error(L); // Raise Lua error
        return 0; // This line is not actually reached because lua_error does a longjmp.
    }

    const char *logMessage = luaL_checkstring(L, 1);
    int logLevel = luaL_checkinteger(L, 2); 
    
    TraceLog(logLevel, "LUA LOG: %s", logMessage);
    
    return 0;
}

void LoadOptionsFromLua(State* state, lua_State* L) {
    LoadWindowConfigFromLua(L, &state->ui.config.window);
    LoadFontConfigFromLua(L, &state->ui);
    LoadThemeFromLua(L, &state->ui);
}

int InitLuaModule(lua_State* L) {
    luaopen_table(L);
        LUA_SET_FUNCTION(lofi_Log, "log");

        LUA_SET_TABLE("log_level",
            LUA_SET_INT(LOG_INFO, "INFO");
            LUA_SET_INT(LOG_DEBUG, "DEBUG");
            LUA_SET_INT(LOG_WARNING, "WARNING");
            LUA_SET_INT(LOG_ERROR, "ERROR");
        );

    lua_setglobal(L, "lofi");

    TraceLog(LOG_DEBUG, "LUA: Initialized the to-be exported lua module.");
    return 1;
}

void RegisterModeManager(ModeManager* modeManager, lua_State* L)
{
    lua_pushlightuserdata(L, modeManager);
    lua_setfield(L, LUA_REGISTRYINDEX, MODE_MANAGER_KEY);

    TraceLog(LOG_DEBUG, "LUA MODES: Pushed the mode manager into user data.");
}

void InitLua(const char* initPath, ModeManager* modeManager, lua_State* L) {
    luaL_openlibs(L);
    InitLuaModule(L);
    RegisterModeManager(modeManager, L);

    // this is more of a hack, but it makes sure the
    // values exist before the file is run
    // The tables are otherwise not present in lua -- 
    // either this or create a separate function to generate the 
    // empty table structure (lofi.opt ...)
    ExportWindowConfigToLua(L, DEFAULT_WINDOW_CONFIG);
    ExportFontConfigToLua(L, (FontConfig) {"default", 20});

    if (luaL_dofile(L, initPath) != LUA_OK) {
        TraceLog(LOG_ERROR, "LUA: Error loading init.lua: %s\n", lua_tostring(L, -1));
        lua_close(L);
    }

    TraceLog(LOG_DEBUG, "Lua initialized, init.lua OK.");
}
