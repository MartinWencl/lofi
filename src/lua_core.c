#include "lua_core.h"
#include <stdlib.h>

#define MODE_MANAGER_KEY "ModeManagerKey"

static void PushAppStateToLua(lua_State* L, AppState* state) {
    lua_newtable(L);
    
    // Push input text
    lua_pushstring(L, state->inputText);
    lua_setfield(L, -2, "input_text");
    
    // Push list items
    lua_newtable(L);
    for (int i = 0; i < state->listCount; i++) {
        lua_pushstring(L, state->listItems[i]);
        lua_rawseti(L, -2, i + 1);
    }
    lua_setfield(L, -2, "list_tems");
    
    // Push other state fields
    lua_pushinteger(L, state->listCount);
    lua_setfield(L, -2, "list_count");
    
    lua_pushinteger(L, state->listScrollIndex);
    lua_setfield(L, -2, "list_scroll_index");
    
    lua_pushinteger(L, state->focus.index);
    lua_setfield(L, -2, "focus");
    TraceLog(LOG_DEBUG, "STATE: Pushed the state to lua.");
}

static void UpdateAppStateFromLua(lua_State* L, AppState* state) {
    // Ensure we're working with a table
    luaL_checktype(L, -1, LUA_TTABLE);
    
    // Update input text
    lua_getfield(L, -1, "input_text");
    if (!lua_isnil(L, -1)) {
        const char* input = lua_tostring(L, -1);
        strncpy(state->inputText, input, INPUT_TEXT_MAX_SIZE - 1);
    }
    lua_pop(L, 1);
    
    // Clear existing list items and reset the count
    ClearListViewExList(&(state->listItems), &(state->listCount));

    // Update list items
    lua_getfield(L, -1, "listItems");
    if (!lua_isnil(L, -1)) {
        int listCount = lua_rawlen(L, -1);
        
        for (int i = 1; i <= listCount; i++) {
            lua_rawgeti(L, -1, i);
            const char* item = lua_tostring(L, -1);
            AddToListViewExList(&(state->listItems), &(state->listCount), item);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
    
    // Update list scroll index
    lua_getfield(L, -1, "list_scroll_index");
    if (!lua_isnil(L, -1)) {
        state->listScrollIndex = lua_tointeger(L, -1);
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

int DispatchLuaEvent(AppState *state, ModeManager *modeManager, AppEventType event) {
    TraceLog(LOG_DEBUG, "LUA EVENT: Dispatching lua event.");
    
    //TEMP: Add switching modes to lua
    modeManager->currentMode = &(modeManager->modes[0]);

    if(modeManager->currentMode == NULL) {
        TraceLog(LOG_ERROR, "LUA EVENT: No current mode set!");
        return 0;
    }

    int ref = modeManager->currentMode->callback_refs[event];

    if (ref == LUA_NOREF || !modeManager->luaState) return 0;

    lua_State* L = modeManager->luaState;
    
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    
    // Push current app state
    PushAppStateToLua(L, state);
    
    // Call the Lua function and check for errors
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        const char *errorMessage = lua_tostring(L, -1);
        TraceLog(LOG_ERROR, "LUA EVENT: Failed to call the corresponding lua function! Error: %s", errorMessage);
        lua_pop(L, 1);
        return 0;
    }

    // Check if the function returned exactly one table
    if (lua_type(L, -1) != LUA_TTABLE) {
        TraceLog(LOG_ERROR, "LUA EVENT: Function did not return a table!");
        lua_pop(L, 1); // Remove the non-table return value
        return 0;
    }
    
    // Update app state from returned table
    UpdateAppStateFromLua(L, state);
    
    // Remove the returned state table
    lua_pop(L, 1);
    
    return 1;
}

static ModeManager* GetModeManager(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, MODE_MANAGER_KEY);
    ModeManager* modeManager = (ModeManager*)lua_touserdata(L, -1);
    lua_pop(L, 1); // Remove the userdata from the stack
    TraceLog(LOG_DEBUG, "LUA MODES: Retrieved mode manager from user data.");
    return modeManager;
}

static int lofi_RegisterMode(lua_State* L) {
    TraceLog(LOG_DEBUG, "LUA MODES: A call to register a mode.");
    luaL_checktype(L, 1, LUA_TTABLE);

    ModeManager *modeManager = GetModeManager(L);

    // Check for name
    lua_getfield(L, 1, "name");
    if (lua_isnil(L, -1)) {
        return luaL_error(L, "Mode must have a name");
    }

    const char* modeName = lua_tostring(L, -1);
    lua_pop(L, 1);
    TraceLog(LOG_DEBUG, "   named: %s", modeName);

    // Check if we have space
    if (modeManager->modeCount >= MAX_MODES) {
        return luaL_error(L, "Maximum number of modes reached");
    }

    Mode* mode = &modeManager->modes[modeManager->modeCount++];
    mode->id = modeManager->modeCount - 1;
    strncpy(mode->name, modeName, MAX_NAME_LENGTH - 1);

    TraceLog(LOG_DEBUG, "   with id: %i", mode->id);

    // Store references for all possible events
    // #define STORE_CALLBACK(event_name) \
        // lua_getfield(L, 1, "on_" #event_name); \
        // mode->ref_##event_name = lua_isfunction(L, -1) ? \
            // luaL_ref(L, LUA_REGISTRYINDEX) : LUA_NOREF;
            
    lua_getfield(L, 1, "on_select");
    mode->callback_refs[EVENT_ITEM_SELECTED] = lua_isfunction(L, -1) ? luaL_ref(L, LUA_REGISTRYINDEX) : LUA_NOREF;
    TraceLog(LOG_DEBUG, "   with on_select: %s", mode->callback_refs[EVENT_ITEM_SELECTED] != LUA_NOREF ? "Yes" : "No");
    lua_getfield(L, 1, "on_search");
    mode->callback_refs[EVENT_SEARCH_TRIGGERED] = lua_isfunction(L, -1) ? luaL_ref(L, LUA_REGISTRYINDEX) : LUA_NOREF;
    TraceLog(LOG_DEBUG, "   with on_search: %s", mode->callback_refs[EVENT_ITEM_SELECTED] != LUA_NOREF ? "Yes" : "No");

    TraceLog(LOG_DEBUG, "LUA MODES: Mode registration complete.");
    return 0;
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
    lua_pushcfunction(L, lofi_RegisterMode);
    lua_setfield(L, -2, "register_mode");

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

lua_State* InitLua(const char* initPath, ModeManager *modeManager) {
    lua_State* L = luaL_newstate();
    if(!L) {
        TraceLog(LOG_ERROR, "LUA: Failed to create a lua state!");
    }
    TraceLog(LOG_DEBUG, "LUA: Created a lua state.");

    luaL_openlibs(L);

    lua_pushlightuserdata(L, modeManager);
    lua_setfield(L, LUA_REGISTRYINDEX, MODE_MANAGER_KEY);
    TraceLog(LOG_DEBUG, "LUA MODES: Pushed the mode manager into user data.");

    // Open the lofi module and push methods/vars/etc
    InitLuaModule(L);
    lua_setglobal(L, "lofi");
    TraceLog(LOG_DEBUG, "LUA: Exported the lua module.");

    // Load and run the init.lua file
    if (luaL_dofile(L, initPath) != LUA_OK) {
        TraceLog(LOG_ERROR, "LUA: Error loading init.lua: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return NULL;
    }

    TraceLog(LOG_DEBUG, "LUA: Loaded init.lua.");

    return L;
}