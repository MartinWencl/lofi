#include "lua_core.h"
#include "utils.h"
// #include "ui/window.h"
// #include "lua/window.h"
#include <stdlib.h>
#include <string.h>


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

int InitLuaModule(lua_State* L) {
    lua_newtable(L);

    // Add event constants
    lua_pushinteger(L, EVENT_NONE);
    lua_setfield(L, -2, "EVENT_NONE");
    lua_pushinteger(L, EVENT_SEARCH_TRIGGERED);
    lua_setfield(L, -2, "EVENT_SEARCH_TRIGGERED");
    lua_pushinteger(L, EVENT_FOCUS_CHANGED);
    lua_setfield(L, -2, "EVENT_FOCUS_CHANGED");
    lua_pushinteger(L, EVENT_ITEM_SELECTED);
    lua_setfield(L, -2, "EVENT_ITEM_SELECTED");
    lua_pushinteger(L, EVENT_SCROLL);
    lua_setfield(L, -2, "EVENT_SCROLL");
    lua_pushinteger(L, EVENT_EXIT);
    lua_setfield(L, -2, "EVENT_EXIT");

    // Add mode management functions
    // lua_pushcfunction(L, lofi_RegisterMode);
    // lua_setfield(L, -2, "register_mode");

    lua_pushcfunction(L, lofi_Log);
    lua_setfield(L, -2, "log");
    // lua_pushcfunction(L, lofi_switch_mode);
    // lua_setfield(L, -2, "switch_mode");

    // log_level
    lua_newtable(L); 

    lua_pushinteger(L, LOG_INFO);
    lua_setfield(L, -2, "INFO");
    lua_pushinteger(L, LOG_DEBUG);
    lua_setfield(L, -2, "DEBUG");
    lua_pushinteger(L, LOG_WARNING);
    lua_setfield(L, -2, "WARNING");
    lua_pushinteger(L, LOG_ERROR);
    lua_setfield(L, -2, "ERROR");

    lua_setfield(L, -2, "log_level");

    TraceLog(LOG_DEBUG, "LUA: Initialized the to-be exported lua module.");
    return 1;
}

void InitLua(const char* initPath, lua_State* L) {
    luaL_openlibs(L);

    // TODO: If modeManager is needed in userdata then put that in a separate
    // func after init
    //
    // lua_pushlightuserdata(L, modeManager);
    // lua_setfield(L, LUA_REGISTRYINDEX, MODE_MANAGER_KEY);
    // TraceLog(LOG_DEBUG, "LUA MODES: Pushed the mode manager into user data.");

    // Open the lofi module and push methods/vars/etc
    InitLuaModule(L);
    lua_setglobal(L, "lofi");
    // this is more of a hack, but it makes sure the
    // values exist before the file is run, so "no need"
    // to worry about crashes on null values
    // ExportWindowConfigToLua(L, DEFAULT_WINDOW_CONFIG);
    TraceLog(LOG_DEBUG, "LUA: Exported the lua module.");

    // Load and run the init.lua file
    if (luaL_dofile(L, initPath) != LUA_OK) {
        TraceLog(LOG_ERROR, "LUA: Error loading init.lua: %s\n", lua_tostring(L, -1));
        lua_close(L);
    }

    TraceLog(LOG_DEBUG, "LUA: Loaded init.lua.");
}
